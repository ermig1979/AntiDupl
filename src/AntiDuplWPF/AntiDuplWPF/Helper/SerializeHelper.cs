using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Xml;
using System.Xml.Serialization;

namespace AntiDuplWPF.Helper
{
    public class SerializeHelper<T>
    {
        /// <summary>
        /// Запись в файл
        /// </summary>
        public static void Save(T target, string xmlFileName)
        {
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

                        if (File.Exists(filePath))
                            if (MessageBox.Show(String.Format("Delete corrupted {0}?", filePath), "Delete target",
                                MessageBoxButton.YesNo, MessageBoxImage.Question) == MessageBoxResult.Yes)
                                File.Delete(filePath);
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
