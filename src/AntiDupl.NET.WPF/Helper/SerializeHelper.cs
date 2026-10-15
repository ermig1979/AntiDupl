using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Xml;
using System.Xml.Serialization;

namespace AntiDupl.NET.WPF.Helper
{
    /// <summary>
    /// Аргументы события о повреждённом файле настроек.
    /// </summary>
    public class FileLoadErrorEventArgs : EventArgs
    {
        /// <summary>Путь к повреждённому файлу.</summary>
        public string FilePath { get; set; }

        /// <summary>Текст ошибки десериализации.</summary>
        public string Message { get; set; }

        public FileLoadErrorEventArgs(string filePath, string message)
        {
            FilePath = filePath;
            Message = message;
        }
    }

    /// <summary>
    /// Негeneric-контейнер события: Load() сериализатора не показывает диалог
    /// (аудит зеркала WF: диалог → событие), а лишь поднимает событие; подписчик в UI-слое
    /// (App.xaml.cs) логирует и показывает сообщение пользователю.
    /// </summary>
    public static class SerializeHelperEvents
    {
        public static event EventHandler<FileLoadErrorEventArgs> LoadErrorOccurred;

        public static void Raise(string filePath, string message)
        {
            if (LoadErrorOccurred != null)
                LoadErrorOccurred(null, new FileLoadErrorEventArgs(filePath, message));
        }
    }

    public class SerializeHelper<T>
    {
        /// <summary>
        /// Запись в файл
        /// </summary>
        public static void Save(T target, string xmlFileName)
        {
            Debug.Assert(target is not null, nameof(target));
            Debug.Assert(!string.IsNullOrWhiteSpace(xmlFileName), nameof(xmlFileName));
            try
            {
                string directory = Path.GetDirectoryName(xmlFileName);
                if (!String.IsNullOrEmpty(directory)
                    && !Directory.Exists(directory))
                    Directory.CreateDirectory(directory);

                XmlSerializer ser = new XmlSerializer(typeof(T));
                //XmlCallbackSerializer ser = new XmlCallbackSerializer(typeof(T));
                //DataContractSerializer ser = new DataContractSerializer(typeof(T));
                //CustomXmlSerializer ser = new CustomXmlSerializer(typeof(T));
                using (TextWriter writer = new StreamWriter(xmlFileName, false))
                {
                    ser.Serialize(writer, target);
                }
            }
            catch (Exception ex)
            {
                StringBuilder sb = new StringBuilder();
                Exception exep = ex;
                for (int i = 0; exep != null; exep = exep.InnerException, i++)
                {
                    sb.AppendLine(string.Format("Type #{0} {1}", i, ex.GetType().ToString()));

                    foreach (System.Reflection.PropertyInfo propInfo in ex.GetType().GetProperties())
                    {
                        string fieldName = string.Format("{0} №{1}", propInfo.Name, i);
                        string fieldValue = string.Format("{0}", propInfo.GetValue(ex, null));

                        // Ignore stack trace + data
                        if (propInfo.Name == "StackTrace"
                            || propInfo.Name == "Data"
                            || string.IsNullOrEmpty(propInfo.Name)
                            || string.IsNullOrEmpty(fieldValue))
                            continue;
                        sb.AppendLine(string.Format("{0}: {1}", fieldName, fieldValue));
                    }
                }

                throw new Exception("Error on serialize: " + ex.Message + Environment.NewLine + sb.ToString());
            }
        }

        public static T Load(string filePath)
        {
            Debug.Assert(!string.IsNullOrWhiteSpace(filePath), nameof(filePath));
            if (File.Exists(filePath))
            {
                FileInfo fileInfo = new FileInfo(filePath);
                if (fileInfo.Extension.Length > 0)
                {
                    try
                    {
                        T t;
                        XmlSerializer ser = new XmlSerializer(typeof(T));
                        using (XmlTextReader reader = new XmlTextReader(filePath))
                        {
                            if (ser.CanDeserialize(reader))
                                t = (T)ser.Deserialize(reader);
                            else
                                throw new Exception(String.Format("Не могу десериализовать файл {0}", filePath));

                            if (reader.ReadState != ReadState.Closed)
                                reader.Close();
                        }
                        ser = null;
                        return t;
                    }
                    catch (Exception ex)
                    {
                        //throw new Exception("Error on deserialize: " + ex.Message);

                        // Диалог не показываем здесь: подписчики LoadErrorOccurred
                        // (App.xaml.cs) решают, как сообщить пользователю.
                        SerializeHelperEvents.Raise(filePath, ex.Message);
                    }
                }
            }
            /*else
            {
                throw new Exception(String.Format("Файл не найден {0}", FilePath));
            }*/
            return default(T);
        }
    }
}
