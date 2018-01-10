
using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Reflection;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace AntiDuplWPF.Core
{
    public class DynamicModule : IDisposable
    {
        public class DynamicModuleApiAttribute : Attribute
        {
        }

        public DynamicModule(string fileName)
        {
            m_fileName = fileName;

            if (string.IsNullOrEmpty(m_fileName))
                throw new Exception(string.Format("Bad library file name '{0}'!", m_fileName));

            try
            {
                m_module = LoadLibrary(m_fileName);
                if (m_module == IntPtr.Zero)
                    throw new Exception(string.Format("Can't load {0} dynamic library!", m_fileName));

                FieldInfo[] fields = this.GetType().GetFields(BindingFlags.Public | BindingFlags.NonPublic | BindingFlags.GetField | BindingFlags.Instance);
                for (int i = 0; i < fields.Length; ++i)
                {
                    FieldInfo field = fields[i];
                    try
                    {
                        object[] attributes = field.GetCustomAttributes(typeof(DynamicModuleApiAttribute), false);
                        if (attributes.Length > 0)
                        {
                            IntPtr address = GetProcAddress(m_module, field.Name);
                            Delegate delegate_ = Marshal.GetDelegateForFunctionPointer(address, field.FieldType);
                            field.SetValue(this, delegate_);
                        }
                    }
                    catch (Exception ex)
                    {
                        Trace.TraceError(ex.ToString());
                    }
                }
            }
            catch
            {
                throw new Exception(string.Format("Can't load {0} dynamic library!", m_fileName));
            }
        }

        ~DynamicModule()
        {
            Dispose();
        }

        public void Dispose()
        {
            if (m_module != IntPtr.Zero)
            {
                FreeLibrary(m_module);
                m_module = IntPtr.Zero;
            }
            GC.SuppressFinalize(this);
        }

        public string FileName { get { return m_fileName; } }

        /************************************ Private Members: ************************************/

        private IntPtr m_module = IntPtr.Zero;
        private string m_fileName;

        [DllImport("kernel32.dll",
            CharSet = CharSet.Ansi,
            CallingConvention = CallingConvention.Winapi,
            EntryPoint = "LoadLibraryA")]
        private static extern IntPtr LoadLibrary(string moduleName);

        [DllImport("kernel32.dll",
            CharSet = CharSet.Ansi,
            CallingConvention = CallingConvention.Winapi,
            EntryPoint = "FreeLibrary")]
        private static extern int FreeLibrary(IntPtr module);


        [DllImport("kernel32.dll",
            CharSet = CharSet.Ansi,
            CallingConvention = CallingConvention.Winapi,
            EntryPoint = "GetProcAddress")]
        private static extern IntPtr GetProcAddress(IntPtr module, string functionName);
    }
}
