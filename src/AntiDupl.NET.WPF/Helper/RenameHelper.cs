using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using AntiDuplWPF.ObjectModel;
using AntiDuplWPF.UndoRedo;
using AntiDuplWPF.ViewModel;

namespace AntiDuplWPF.Helper
{
    public class RenameHelper
    {
        public static void FillMenu(DuplPairViewModel res, out string firstToSecond, out string secondToFirst,
            out IUCommand firstToSecondCommand, out IUCommand secondToFirstCommand)
        {
            string menuText;
            if (Path.GetDirectoryName(res.FirstFile.Path) == Path.GetDirectoryName(res.SecondFile.Path))
            {
                menuText = "Переименовать {0} в {1}";
                string firstNewName = GetSimilarName(res.SecondFile.Path, res.FirstFile.Path);
                firstToSecond = String.Format(menuText, Path.GetFileName(res.FirstFile.Path), Path.GetFileName(firstNewName));
                firstToSecondCommand = new RenameLikeCommand(res, firstNewName, PositionInDuplPair.First);

                string secondNewName = GetSimilarName(res.FirstFile.Path, res.SecondFile.Path);
                secondToFirst = String.Format(menuText, Path.GetFileName(res.SecondFile.Path), Path.GetFileName(secondNewName));
                secondToFirstCommand = new RenameLikeCommand(res, secondNewName, PositionInDuplPair.Second);
            }
            else
            {
                menuText = "Переместить в {0} и переименовать {1} в {2}";
                string firstNewName = GetSimilarName(res.SecondFile.Path, res.FirstFile.Path);
                firstToSecond = String.Format(menuText, Path.GetDirectoryName(res.SecondFile.Path),
                    Path.GetFileName(res.FirstFile.Path), Path.GetFileName(firstNewName));
                firstToSecondCommand = new RenameLikeCommand(res, firstNewName, PositionInDuplPair.First);
                string secondNewName = GetSimilarName(res.FirstFile.Path, res.SecondFile.Path);
                secondToFirst = String.Format(menuText, Path.GetDirectoryName(res.FirstFile.Path),
                    Path.GetFileName(res.SecondFile.Path), Path.GetFileName(secondNewName));
                secondToFirstCommand = new RenameLikeCommand(res, secondNewName, PositionInDuplPair.Second);
            }
        }

        /// <summary>
        /// Переименовать файл с именем похожим на переданное. Если файл с таким именем уже существует, то переименовывается с похожим именем.
        /// </summary>
        /// <param name="targetPath">Целевой путь</param>
        /// <param name="currentName">Текущее имя файла</param>
        /// <returns></returns>
        public static string GetSimilarName(string targetPath, string currentName)
        {
            ulong digit = 0;
            string nameWithoutNumber = String.Empty;
            int leadingZero = 0;

            digit = GetDigit(Path.GetFileNameWithoutExtension(targetPath), out nameWithoutNumber, out leadingZero);

            if (digit == 0)
                targetPath = GetNewNameForFileAdd(targetPath, 2);
            else
                targetPath = GetNewNameForFileDig(Path.Combine(Directory.GetParent(targetPath).ToString() + "\\", nameWithoutNumber),
                                                leadingZero,
                                                digit + 1,
                                                Path.GetExtension(targetPath),
                                                targetPath,
                                                currentName);
            return targetPath;
        }

        /// <summary>
        /// Check is in file name number separated by the non digit character from remaining part of file name. Returns number or 0 in case of failure.
        /// </summary>
        /// <param name="name">file name</param>
        /// <param name="pathWithoutNumber">Output file name without number and "obj"</param>
        /// <param name="numOfZero">Number of leading numOfZero</param>
        /// <returns>0 or the received number</returns>
        private static ulong GetDigit(string name, out string nameWithoutDigit, out int numOfZero)
        {
            int length = name.Length;
            //Находим первый не числовой символ с конца
            bool canRename;
            int digitPos = length;
            for (int u = length - 1; u >= 0; u--)
                if (!char.IsDigit(name[u]))
                {
                    digitPos = u;
                    break;
                }
            if (digitPos < length) //если цифра найдена
                canRename = true;
            else
                canRename = false;

            ulong result = 0;
            numOfZero = 0;
            if (canRename)
            {
                string forParsing = name.Substring(digitPos + 1);
                ulong.TryParse(forParsing, out result);
                numOfZero = forParsing.Length - result.ToString().Length;
            }

            if (digitPos < length)
                nameWithoutDigit = name.Substring(0, digitPos + 1);
            else
                nameWithoutDigit = string.Empty;
            return result;
        }

        /// <summary>
        /// Adding to number file name in a case when in it it wasn't.
        /// </summary>
        /// <param name="pathWithoutNumber">Old name</param>
        /// <param name="digit">Number</param>
        /// <returns>New name</returns>
        private static string GetNewNameForFileAdd(string oldName, ulong i)
        {
            string newName = string.Format("{0}\\{1}_{2}{3}", Directory.GetParent(oldName).ToString(), Path.GetFileNameWithoutExtension(oldName), i, Path.GetExtension(oldName));
            if (File.Exists(newName))
            {
                i = i + 1;
                newName = GetNewNameForFileAdd(oldName, i);
            }
            return newName;
        }

        /// <summary>
        /// Adding to number file name in a case when in it was number.
        /// </summary>
        /// <param name="pathWithoutNumber">Old name</param>
        /// <param name="digit">Number</param>
        /// <param name="extension">Filename extension</param>
        /// <returns>New name</returns>
        private static string GetNewNameForFileDig(string pathWithoutNumber, int zero, ulong digit, string extension, string sourceName, string currentName)
        {
            string newName = String.Empty;
            if (digit.ToString().Length > (digit - 1).ToString().Length) //не добавлять один ноль если цифра удленилась
                zero--;

            StringBuilder builder = new StringBuilder(pathWithoutNumber);
            for (int j = 0; j < zero; j++)
                builder.Append("0");
            builder.Append(digit); //incresed number
            builder.Append(extension);
            newName = builder.ToString();

            if (File.Exists(newName))
            {
                if (string.Compare(newName, currentName, true) != 0)
                    newName = GetNewNameForFileAdd(sourceName, 2);
            }
            return newName;
        }
    }
}
