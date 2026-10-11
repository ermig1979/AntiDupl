using System;
using System.Collections.Generic;
using System.Linq;
using System.Windows;
using System.Windows.Input;

namespace AntiDupl.NET.WPF.Model
{
    /// <summary>
    /// Действия, привязываемые к горячим клавишам (зеркало WF HotKeyOptions.Action,
    /// порядок значений совпадает с WF-персистентностью).
    /// </summary>
    public enum HotKeyAction
    {
        DeleteDefect,
        DeleteFirst,
        DeleteSecond,
        DeleteBoth,
        RenameFirstToSecond,
        RenameSecondToFirst,
        Mistake,
        ShowNeighbours,
        OpenImageDiff,
        Size
    }

    /// <summary>
    /// Одна привязка: действие + клавиша. Key — строка вида "NumPad1", "Q",
    /// "Ctrl+Q", "Shift+Ctrl+Q"; пустая строка = привязка отключена.
    /// </summary>
    [Serializable]
    public class HotKeyActionItem
    {
        public HotKeyAction Action { get; set; }

        public string Key { get; set; }

        public HotKeyActionItem()
        {
        }

        public HotKeyActionItem(HotKeyAction action, string key)
        {
            Action = action;
            Key = key ?? "";
        }
    }

    /// <summary>
    /// Набор горячих клавиш (зеркало WF HotKeyOptions): дефолты, валидность
    /// (резервированные клавиши/комбинации + коллизии), преобразование в KeyGesture.
    /// Сохраняется в configuration.xml.
    /// </summary>
    [Serializable]
    public class HotKeyOptions
    {
        public List<HotKeyActionItem> Items { get; set; }

        public HotKeyOptions()
        {
            Check();
        }

        public HotKeyOptions(HotKeyOptions options)
        {
            if (options != null && options.Items != null)
            {
                Items = options.Items.Select(i => new HotKeyActionItem(i.Action, i.Key)).ToList();
            }
            Check();
        }

        /// <summary>
        /// Дополняет пропущенные/сменившие размер список значениями по умолчанию
        /// (зеркало WF Check()/EnsureDefaultIfEmpty).
        /// </summary>
        public void Check()
        {
            int size = (int)HotKeyAction.Size;
            List<HotKeyActionItem> previous = null;
            if (Items != null && Items.Count == size)
            {
                bool complete = true;
                for (int i = 0; i < size; i++)
                {
                    if (Items.All(item => item == null || item.Action != (HotKeyAction)i))
                    {
                        complete = false;
                        break;
                    }
                }
                if (complete)
                    previous = Items;
            }

            Items = new List<HotKeyActionItem>();
            for (int i = 0; i < size; i++)
                Items.Add(new HotKeyActionItem((HotKeyAction)i, ""));

            if (previous != null)
            {
                foreach (var item in previous)
                {
                    if (item == null)
                        continue;
                    Items.First(i => i.Action == item.Action).Key = item.Key ?? "";
                }
            }

            // Те же дефолты, что и в WF HotKeyOptions.Check().
            EnsureDefaultIfEmpty(HotKeyAction.DeleteDefect, "NumPad1");
            EnsureDefaultIfEmpty(HotKeyAction.DeleteFirst, "NumPad1");
            EnsureDefaultIfEmpty(HotKeyAction.DeleteSecond, "NumPad2");
            EnsureDefaultIfEmpty(HotKeyAction.DeleteBoth, "NumPad3");
            EnsureDefaultIfEmpty(HotKeyAction.RenameFirstToSecond, "NumPad4");
            EnsureDefaultIfEmpty(HotKeyAction.RenameSecondToFirst, "NumPad6");
            EnsureDefaultIfEmpty(HotKeyAction.Mistake, "NumPad5");
            EnsureDefaultIfEmpty(HotKeyAction.ShowNeighbours, "Ctrl+Q");
            EnsureDefaultIfEmpty(HotKeyAction.OpenImageDiff, "Ctrl+D");
        }

        void EnsureDefaultIfEmpty(HotKeyAction action, string defaultKey)
        {
            var item = Items.First(i => i.Action == action);
            if (String.IsNullOrEmpty(item.Key) && !IsKeyUsed(defaultKey, action))
                item.Key = defaultKey;
        }

        bool IsKeyUsed(string key, HotKeyAction exceptAction)
        {
            if (String.IsNullOrEmpty(key))
                return false;
            return Items.Any(i => i.Action != exceptAction
                && !String.IsNullOrEmpty(i.Key)
                && String.Equals(i.Key, key, StringComparison.OrdinalIgnoreCase));
        }

        public HotKeyActionItem GetItem(HotKeyAction action)
        {
            Check();
            return Items.First(i => i.Action == action);
        }

        public string GetKey(HotKeyAction action)
        {
            return GetItem(action).Key;
        }

        public void SetKey(HotKeyAction action, string key)
        {
            GetItem(action).Key = key ?? "";
        }

        /// <summary>Клавиша по умолчанию для действия (зеркало WF SetDefault(action)).</summary>
        public string GetDefaultKeyFor(HotKeyAction action)
        {
            switch (action)
            {
                case HotKeyAction.DeleteDefect:
                    return "NumPad1";
                case HotKeyAction.DeleteFirst:
                    return "NumPad1";
                case HotKeyAction.DeleteSecond:
                    return "NumPad2";
                case HotKeyAction.DeleteBoth:
                    return "NumPad3";
                case HotKeyAction.RenameFirstToSecond:
                    return "NumPad4";
                case HotKeyAction.RenameSecondToFirst:
                    return "NumPad6";
                case HotKeyAction.Mistake:
                    return "NumPad5";
                case HotKeyAction.ShowNeighbours:
                    return "Ctrl+Q";
                case HotKeyAction.OpenImageDiff:
                    return "Ctrl+D";
                default:
                    return "";
            }
        }

        /// <summary>Строка клавиши в формате для отображения/сохранения ("Ctrl+Q", "NumPad1").</summary>
        public string ToDisplay(string key)
        {
            var gesture = Parse(key);
            return gesture != null ? ToDisplayString(gesture) : "";
        }

        public void SetDefault()
        {
            Items = new List<HotKeyActionItem>();
            for (int i = 0; i < (int)HotKeyAction.Size; i++)
                Items.Add(new HotKeyActionItem((HotKeyAction)i, ""));
            Check();
        }

        static readonly Key[] ReservedKeys =
        {
            Key.Up, Key.Down, Key.PageUp, Key.PageDown, Key.Home, Key.End
        };

        /// <summary>
        /// Валидность одной привязки (зеркало WF Valid(action)): None/пусто — валидно;
        /// резервированные клавиши и комбинации Ctrl+A/C/Z/Y — нет; коллизии между
        /// действиями — нет (со спец-случаем DeleteDefect/Mistake как в WF).
        /// </summary>
        public bool Valid(HotKeyAction action)
        {
            Check();
            var gesture = Parse(GetItem(action).Key);
            if (gesture == null)
                return true;

            if (ReservedKeys.Contains(gesture.Key))
                return false;

            // Ctrl+A / Ctrl+C / Ctrl+Z / Ctrl+Y зарезервированы приложением.
            if (gesture.Modifiers == ModifierKeys.Control)
            {
                if (gesture.Key == Key.A || gesture.Key == Key.C
                    || gesture.Key == Key.Z || gesture.Key == Key.Y)
                    return false;
            }

            if (action == HotKeyAction.DeleteDefect)
            {
                if (Equals(GetKey(HotKeyAction.Mistake), GetKey(action)))
                    return false;
            }
            else if (action == HotKeyAction.Mistake)
            {
                for (int i = (int)HotKeyAction.DeleteDefect; i < (int)HotKeyAction.Mistake; i++)
                {
                    if (Equals(GetKey((HotKeyAction)i), GetKey(action)))
                        return false;
                }
            }
            else
            {
                for (int i = (int)HotKeyAction.DeleteFirst; i < (int)HotKeyAction.Size; i++)
                {
                    var a = (HotKeyAction)i;
                    if (a != action && Equals(GetKey(a), GetKey(action)))
                        return false;
                }
            }

            return true;
        }

        public bool Valid()
        {
            for (int i = 0; i < (int)HotKeyAction.Size; i++)
            {
                if (!Valid((HotKeyAction)i))
                    return false;
            }
            return true;
        }

        /// <summary>
        /// Парсит строку клавиши ("Ctrl+Shift+Q", "NumPad1") в KeyGesture.
        /// null = пусто/не распознано.
        /// </summary>
        public static KeyGesture Parse(string key)
        {
            if (String.IsNullOrWhiteSpace(key))
                return null;

            string[] parts = key.Split('+');
            Key keyPart;
            if (!Enum.TryParse(parts[parts.Length - 1], true, out keyPart))
                return null;

            ModifierKeys modifiers = ModifierKeys.None;
            for (int i = 0; i < parts.Length - 1; i++)
            {
                switch (parts[i].Trim().ToLowerInvariant())
                {
                    case "ctrl":
                    case "control":
                        modifiers |= ModifierKeys.Control;
                        break;
                    case "shift":
                        modifiers |= ModifierKeys.Shift;
                        break;
                    case "alt":
                        modifiers |= ModifierKeys.Alt;
                        break;
                    default:
                        return null;
                }
            }

            try
            {
                return new KeyGesture(keyPart, modifiers);
            }
            catch (ArgumentException)
            {
                return null;
            }
        }

        public static string ToDisplayString(KeyGesture gesture)
        {
            if (gesture == null)
                return "";
            List<string> parts = new List<string>();
            if ((gesture.Modifiers & ModifierKeys.Alt) != 0)
                parts.Add("Alt");
            if ((gesture.Modifiers & ModifierKeys.Control) != 0)
                parts.Add("Ctrl");
            if ((gesture.Modifiers & ModifierKeys.Shift) != 0)
                parts.Add("Shift");
            if ((gesture.Modifiers & ModifierKeys.Windows) != 0)
                parts.Add("Win");
            parts.Add(gesture.Key.ToString());
            return string.Join("+", parts);
        }
    }
}