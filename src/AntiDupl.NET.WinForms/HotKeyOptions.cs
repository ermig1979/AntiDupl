/*
* AntiDupl.NET Program (http://ermig1979.github.io/AntiDupl).
*
* Copyright (c) 2002-2018 Yermalayeu Ihar.
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
using System.Collections.Generic;
using System.Text;
using System.Windows.Forms;

namespace AntiDupl.NET.WinForms
{
    public class HotKeyOptions
    {
        public enum Action
        {
            CurrentDefectDelete,
            CurrentDuplPairDeleteFirst,
            CurrentDuplPairDeleteSecond,
            CurrentDuplPairDeleteBoth,
            CurrentDuplPairRenameFirstToSecond,
            CurrentDuplPairRenameSecondToFirst,
            CurrentMistake,
            ShowNeighbours,
            OpenImageDiff,
            Size
        }
        
        public Keys[] keys;
        
        public HotKeyOptions()
        {
            keys = null;
            Check();
        }
    
        public HotKeyOptions(HotKeyOptions options)
        {
            keys = options != null && options.keys != null ? (Keys[])options.keys.Clone() : null;
            Check();
        }

        public void Check()
        {
            int size = (int)Action.Size;
            if (keys != null && keys.Length == size)
                return;

            Keys[] previous = keys;
            keys = new Keys[size];

            if (previous != null)
            {
                for (int i = 0; i < Math.Min(previous.Length, size); ++i)
                    keys[i] = previous[i];
            }

            EnsureDefaultIfEmpty(Action.CurrentDefectDelete, Keys.NumPad1);
            EnsureDefaultIfEmpty(Action.CurrentDuplPairDeleteFirst, Keys.NumPad1);
            EnsureDefaultIfEmpty(Action.CurrentDuplPairDeleteSecond, Keys.NumPad2);
            EnsureDefaultIfEmpty(Action.CurrentDuplPairDeleteBoth, Keys.NumPad3);
            EnsureDefaultIfEmpty(Action.CurrentDuplPairRenameFirstToSecond, Keys.NumPad4);
            EnsureDefaultIfEmpty(Action.CurrentDuplPairRenameSecondToFirst, Keys.NumPad6);
            EnsureDefaultIfEmpty(Action.CurrentMistake, Keys.NumPad5);
            EnsureDefaultIfEmpty(Action.ShowNeighbours, Keys.Control | Keys.Q);
            EnsureDefaultIfEmpty(Action.OpenImageDiff, Keys.Control | Keys.D);
        }

        private void EnsureDefaultIfEmpty(Action action, Keys defaultKey)
        {
            int index = (int)action;
            if (keys[index] == Keys.None && !IsKeyUsed(defaultKey, action))
                keys[index] = defaultKey;
        }

        private bool IsKeyUsed(Keys key, Action exceptAction)
        {
            if (key == Keys.None)
                return false;

            for (int i = 0; i < keys.Length; ++i)
            {
                if (i != (int)exceptAction && keys[i] == key)
                    return true;
            }
            return false;
        }

        public Keys Get(Action action)
        {
            Check();
            return keys[(int)action];
        }
        
        public void SetDefault()
        {
            keys = new Keys[(int)Action.Size];
            keys[(int)Action.CurrentDefectDelete] = Keys.NumPad1;
            keys[(int)Action.CurrentDuplPairDeleteFirst] = Keys.NumPad1;
            keys[(int)Action.CurrentDuplPairDeleteSecond] = Keys.NumPad2;
            keys[(int)Action.CurrentDuplPairDeleteBoth] = Keys.NumPad3;
            keys[(int)Action.CurrentDuplPairRenameFirstToSecond] = Keys.NumPad4;
            keys[(int)Action.CurrentDuplPairRenameSecondToFirst] = Keys.NumPad6;
            keys[(int)Action.CurrentMistake] = Keys.NumPad5;
            keys[(int)Action.ShowNeighbours] = Keys.Control | Keys.Q;
            keys[(int)Action.OpenImageDiff] = Keys.Control | Keys.D;
        }

        public void SetDefault(Action action)
        {
            int i = 1;
            for (Keys key = Keys.NumPad1; key < Keys.NumPad5; key++, i++)
            {
                if ((int)action == i)
                {
                    keys[(int)action] = key;
                    break;
                }
            }
            if (action == Action.CurrentDefectDelete)
                keys[(int)Action.CurrentDefectDelete] = Keys.NumPad1;
            if (action == Action.CurrentMistake)
                keys[(int)Action.CurrentMistake] = Keys.NumPad5;
            if (action == Action.CurrentDuplPairRenameSecondToFirst)
                keys[(int)Action.CurrentDuplPairRenameSecondToFirst] = Keys.NumPad6;
            if (action == Action.ShowNeighbours)
                keys[(int)Action.ShowNeighbours] = Keys.Control | Keys.Q;
            if (action == Action.OpenImageDiff)
				keys[(int)Action.OpenImageDiff] = Keys.Control | Keys.D;
            /*for(Keys key = Keys.NumPad1; key < Keys.NumPad7; key++)
            {
                keys[(int)action] = key;
                if(Valid(action)) 
                    break;
            }*/
        }
        
        public void CopyTo(ref HotKeyOptions options)
        {
            Check();
            options.Check();
            if (keys.Length != options.keys.Length)
                options.keys = new Keys[(int)Action.Size];
            for (int i = 0; i < keys.Length; ++i)
                options.keys[i] = keys[i];
        }

        public bool Equals(HotKeyOptions options)
        {
            Check();
            if (options == null)
                return false;
            options.Check();
            if (keys.Length != options.keys.Length)
                return false;
            for (int i = 0; i < keys.Length; ++i)
                if(options.keys[i] != keys[i])
                    return false;
            return true;
        }

        public bool Valid(Action action)
        {
            Check();
            KeyEventArgs key = new KeyEventArgs(keys[(int)action]);
            if(key.KeyData == Keys.None)
            {
                return true;
            }
            for(int i = 0; i < m_reservedKeys.Length; i++)
            {
                if(key.KeyCode == m_reservedKeys[i])
                {
                    return false;
                }
            }
            for (int i = 0; i < m_reservedKeyCombinations.Length; i++)
            {
                if (key.KeyData == m_reservedKeyCombinations[i])
                {
                    return false;
                }
            }
            
            if(action == Action.CurrentDefectDelete)
            {
                if(key.KeyData == keys[(int)Action.CurrentMistake])
                {
                    return false;
                }
            }
            else if (action == Action.CurrentMistake)
            {
                for (Action a = Action.CurrentDefectDelete; a < Action.CurrentMistake; a++)
                {
                    if (key.KeyData == keys[(int)a])
                    {
                        return false;
                    }
                }
            }
            else
            {
                for (Action a = Action.CurrentDuplPairDeleteFirst; a < Action.Size; a++)
                {
                    if (a != action && key.KeyData == keys[(int)a])
                    {
                        return false;
                    }
                }
            }

            return true;
        }

        public bool Valid()
        {
            for (Action action = Action.CurrentDefectDelete; action < Action.Size; action++)
            {
                if(!Valid(action))
                {
                    return false;
                }
            }
            return true;
        }
        
        private static Keys[] m_reservedKeyCombinations =
        {
            Keys.A | Keys.Control,
            Keys.C | Keys.Control,
            Keys.Z | Keys.Control,
            Keys.Y | Keys.Control
        };

        private static Keys[] m_reservedKeys =
        {
            Keys.Up, 
            Keys.Down,
            Keys.PageUp,
            Keys.PageDown,
            Keys.Home,
            Keys.End             
        };
    }
}
