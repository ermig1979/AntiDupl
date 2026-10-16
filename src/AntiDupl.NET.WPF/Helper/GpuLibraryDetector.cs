using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using Microsoft.Win32;

namespace AntiDupl.NET.WPF.Helper
{
    /// <summary>
    /// Автодетекция каталогов CUDA/cuDNN (бинарных bin-папок) в типовых местах установки:
    /// CUDA — реестр NVIDIA (GPU Computing Toolkit, в т.ч. нестандартные диски) и
    /// Program Files; cuDNN — типовая раскладка NVIDIA (zip v9: bin\12.x\x64, v8: bin\x64),
    /// корневые каталоги дисков и неглубокий поиск по дискам (до 3 уровней) до папки с
    /// cudnn*.dll. Используется окном «GPU недоступен» для предзаполнения полей, когда
    /// пути в настройках не заданы; итог выбора всегда подтверждает пользователь.
    /// </summary>
    public static class GpuLibraryDetector
    {
        /// <summary>
        /// Каталог bin CUDA: сначала реестр (все установленные версии, выбирается
        /// старшая с наличным cudart64_*.dll), затем Program Files (оба, v* с cudart).
        /// Null, если ничего не найдено.
        /// </summary>
        public static string DetectCudaPath()
        {
            // 1) Реестр: HKLM\SOFTWARE\NVIDIA Corporation\GPU Computing Toolkit\CUDA\vX.Y (BinaryPath).
            // Покрывает установку в нестандартный диск (D:\cuda и т.п.).
            try
            {
                using (RegistryKey toolkit =
                    Registry.LocalMachine.OpenSubKey(@"SOFTWARE\NVIDIA Corporation\GPU Computing Toolkit\CUDA"))
                {
                    if (toolkit != null)
                    {
                        string best = null;
                        double bestVersion = -1;
                        foreach (string subName in toolkit.GetSubKeyNames())
                        {
                            if (!subName.StartsWith("v", StringComparison.OrdinalIgnoreCase))
                                continue;
                            double version;
                            if (!TryParseCudaVersion(subName, out version))
                                continue;
                            using (RegistryKey versionKey = toolkit.OpenSubKey(subName))
                            {
                                string binPath = versionKey?.GetValue("BinaryPath") as string;
                                if (string.IsNullOrWhiteSpace(binPath))
                                    continue;
                                if (HasCudart(binPath) && version > bestVersion)
                                {
                                    bestVersion = version;
                                    best = binPath;
                                }
                            }
                        }
                        if (best != null)
                            return best;
                    }
                }
            }
            catch
            {
                // Реестр недоступен — переходим к поиску по типичным каталогам.
            }

            // 2) Типовые каталоги установки (Program Files, в т.ч. x86-версия реестра на 64-битных).
            string[] roots =
            {
                @"C:\Program Files\NVIDIA GPU Computing Toolkit\CUDA",
                @"C:\Program Files (x86)\NVIDIA GPU Computing Toolkit\CUDA"
            };
            foreach (string root in roots)
            {
                string best = BestCudaVersionDirectory(root);
                if (best != null)
                    return best;
            }
            return null;
        }

        /// <summary>
        /// Каталог bin cuDNN: типовая раскладка NVIDIA (C:\Program Files\NVIDIA\CUDNN\...),
        /// корневые каталоги дисков (cudnn\bin) и неглубокий поиск по дискам до папки с
        /// cudnn*.dll (до 3 уровней). Null, если ничего не найдено.
        /// </summary>
        public static string DetectCudnnPath()
        {
            // 1) Типовая раскладка: ...CUDNN\vx.y\bin\12.x\x64 (zip v9) или ...CUDNN\vx.y\bin\x64 (v8).
            string[] roots =
            {
                @"C:\Program Files\NVIDIA\CUDNN",
                @"C:\Program Files (x86)\NVIDIA\CUDNN"
            };
            foreach (string root in roots)
            {
                if (!Directory.Exists(root))
                    continue;
                foreach (string versionDir in SafeGetDirectories(root))
                {
                    string found = FindCudnnBinInTree(versionDir, maxDepth: 3);
                    if (found != null)
                        return found;
                }
            }

            // 2) Корневые каталоги дисков: ...\cudnn\bin и т.п. (популярный способ распаковки zip).
            // 3) Неглубокий поиск по диску (до 3 уровней) — поиск медленный, только если типовых мест нет.
            foreach (DriveInfo drive in SafeFixedDrives())
            {
                string root = drive.RootDirectory.FullName;
                foreach (string topLevel in SafeGetDirectories(root))
                {
                    if (!topLevel.EndsWith("cudnn", StringComparison.OrdinalIgnoreCase))
                        continue;
                    string topFound = FindCudnnBinInTree(topLevel, maxDepth: 2);
                    if (topFound != null)
                        return topFound;
                }

                string found = FindCudnnDirectoryOnDrive(drive);
                if (found != null)
                    return found;
            }
            return null;
        }

        /// <summary>Правда ли, что в каталоге есть cudart64_*.dll (маркер bin CUDA).</summary>
        public static bool HasCudart(string binPath)
        {
            try
            {
                return Directory.Exists(binPath) &&
                    Directory.EnumerateFiles(binPath, "cudart64_*.dll").Any();
            }
            catch
            {
                return false;
            }
        }

        static IEnumerable<DriveInfo> SafeFixedDrives()
        {
            try
            {
                return DriveInfo.GetDrives()
                    .Where(d => d.IsReady && d.DriveType == DriveType.Fixed)
                    .ToList();
            }
            catch
            {
                return Enumerable.Empty<DriveInfo>();
            }
        }

        static IEnumerable<string> SafeGetDirectories(string path)
        {
            try
            {
                return Directory.GetDirectories(path).ToList();
            }
            catch
            {
                return Enumerable.Empty<string>();
            }
        }

        /// <summary>Старшая установленная версия CUDA в каталоге (v12.4 &gt; v11.8) с наличным cudart.</summary>
        static string BestCudaVersionDirectory(string root)
        {
            if (!Directory.Exists(root))
                return null;
            string best = null;
            double bestVersion = -1;
            foreach (string versionDir in SafeGetDirectories(root))
            {
                string versionName = Path.GetFileName(versionDir);
                if (!versionName.StartsWith("v", StringComparison.OrdinalIgnoreCase))
                    continue;
                double version;
                if (!TryParseCudaVersion(versionName, out version))
                    continue;
                string bin = Path.Combine(versionDir, "bin");
                if (HasCudart(bin) && version > bestVersion)
                {
                    bestVersion = version;
                    best = bin;
                }
            }
            return best;
        }

        /// <summary>"v12.4" → 12.4 (для сравнения версий).</summary>
        static bool TryParseCudaVersion(string name, out double version)
        {
            version = 0;
            if (name.Length < 2 || !char.IsDigit(name[1]))
                return false;
            string digits = "";
            for (int i = 1; i < name.Length && (char.IsDigit(name[i]) || name[i] == '.'); i++)
                digits += name[i];
            return double.TryParse(digits.Replace('.', ','), out version);
        }

        /// <summary>
        /// Ищет в дереве (не глубле maxDepth) первую папку, содержащую cudnn*.dll
        /// (cudnn64_8.dll / cudnn_ops64_9.dll и т.п.) — это и есть bin cuDNN.
        /// </summary>
        static string FindCudnnBinInTree(string root, int maxDepth)
        {
            List<string> currentLevel = new List<string> { root };
            for (int depth = 0; depth <= maxDepth && currentLevel.Count > 0; depth++)
            {
                List<string> nextLevel = new List<string>();
                foreach (string dir in currentLevel)
                {
                    if (ContainsCudnn(dir))
                        return dir;
                    if (depth < maxDepth)
                        nextLevel.AddRange(SafeGetDirectories(dir));
                }
                currentLevel = nextLevel;
            }
            return null;
        }

        static bool ContainsCudnn(string dir)
        {
            try
            {
                return Directory.EnumerateFiles(dir, "cudnn*.dll").Any();
            }
            catch
            {
                return false;
            }
        }

        /// <summary>
        /// Неглубокий обход диска (до 3 уровней каталогов) в поиске папки с cudnn*.dll.
        /// Пропускает системные каталоги (Windows, ProgramData, $Recycle.Bin, Users-системные).
        /// </summary>
        static string FindCudnnDirectoryOnDrive(DriveInfo drive)
        {
            string root = drive.RootDirectory.FullName;
            List<string> currentLevel = new List<string>();
            foreach (string dir in SafeGetDirectories(root))
            {
                string name = Path.GetFileName(dir.TrimEnd('\\', '/'));
                if (IsSystemFolder(name))
                    continue;
                currentLevel.Add(dir);
            }

            for (int depth = 1; depth <= 3 && currentLevel.Count > 0; depth++)
            {
                List<string> nextLevel = new List<string>();
                foreach (string dir in currentLevel)
                {
                    if (ContainsCudnn(dir))
                        return dir;
                    if (depth < 3)
                    {
                        foreach (string sub in SafeGetDirectories(dir))
                        {
                            string name = Path.GetFileName(sub.TrimEnd('\\', '/'));
                            if (IsSystemFolder(name))
                                continue;
                            nextLevel.Add(sub);
                        }
                    }
                }
                currentLevel = nextLevel;
            }
            return null;
        }

        static bool IsSystemFolder(string name)
        {
            return name.StartsWith("$", StringComparison.Ordinal) ||
                string.Equals(name, "Windows", StringComparison.OrdinalIgnoreCase) ||
                string.Equals(name, "ProgramData", StringComparison.OrdinalIgnoreCase) ||
                string.Equals(name, "Recovery", StringComparison.OrdinalIgnoreCase) ||
                string.Equals(name, "System Volume Information", StringComparison.OrdinalIgnoreCase) ||
                string.Equals(name, "PerfLogs", StringComparison.OrdinalIgnoreCase);
        }
    }
}