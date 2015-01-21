/*
* AntiDupl.NET Program.
*
* Copyright (c) 2015 Borisov Dmitry.
*
* Permission is hereby granted, free of charge, to any person obtaining a copy 
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell 
* copies of the Software, and to permit persons to whom the Software is 
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in 
* all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR 
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE 
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER 
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*/

using System;
using System.IO;
using System.Text;

namespace AntiDupl.NET
{
    class Rename
    {
        /// <summary>
        /// Переименовать файл с именем похожим на переданное. Если файл с таким именем уже существует, то переименовывается с похожим именем.
        /// </summary>
        /// <param name="targetPath">Целевой путь</param>
        /// <param name="currentName">Текущее имя файла</param>
        /// <returns></returns>
        public static string SimilarRename(string targetPath, string currentName)
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
        /// <param name="pathWithoutNumber">Output file name without number and "_"</param>
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
