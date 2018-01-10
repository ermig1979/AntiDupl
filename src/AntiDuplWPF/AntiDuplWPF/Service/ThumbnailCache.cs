using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Media.Imaging;
using AntiDuplWPF.Model;

namespace AntiDuplWPF.Service
{
    class ThumbnailCache
    {
        //public int MaxCachedThumbnail = 400;

        private List<string> _keyList = new List<string>(); //Keylist for managing the oldest to newest image
        private Dictionary<string, BitmapImage> _thumbnailDict = new Dictionary<string, BitmapImage>(); //Hashtable for storing images
        private IConfigurationModel _configuration;

        public ThumbnailCache(IConfigurationModel configuration)
        {
            _configuration = configuration;
        }

        /// <summary>Get thumbnail</summary>
        public BitmapImage GetThumbnail(string path)
        {
            lock (_thumbnailDict)
            {
                if (_thumbnailDict.ContainsKey(path))
                {
                    if (_keyList.IndexOf(path) != _keyList.Count - 1)
                    {
                        _keyList.Remove(path); //Put key to the end of list
                        _keyList.Add(path);
                    }
                    return _thumbnailDict[path];
                }
                else
                    return null;
            }
        }

        /// <summary>Add thumbnail to thumbnail dict</summary>
        public void AddThumbnail(string key, BitmapImage thumbnail)
        {
            //Delete the oldest thumbnail
            lock (_thumbnailDict)
            {
                if (_keyList.Count >= _configuration.MaxCachedThumbnail)
                {
                    Debug.WriteLine("В кеше превышено максимальное количество эскизов " + _configuration.MaxCachedThumbnail + " Удаялем.");
                    string oldestKey = _keyList[0];
                    _keyList.RemoveAt(0);
                    _thumbnailDict.Remove(oldestKey);
                    GC.Collect(GC.MaxGeneration);
                }
                _thumbnailDict.Add(key, thumbnail);
                _keyList.Add(key);
            }
        }

        /// <summary></summary>
        /// <param name="keyObj"></param>
        /// <returns></returns>
        public bool HasThumbnail(string key)
        {
            lock (_thumbnailDict)
            {
                if (_thumbnailDict.ContainsKey(key))
                    return true;
                else
                    return false;
            }
        }

        /// <summary>Total number of cached thumbnail</summary>
        public int Cached
        {
            get
            {
                lock (_thumbnailDict)
                    return _keyList.Count;
            }
        }

        public void Clear()
        {
            lock (_thumbnailDict)
            {
                _thumbnailDict.Clear();
                _keyList.Clear();
            }
        }
    }
}
