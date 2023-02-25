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
using System.Drawing;

namespace AntiDupl.NET
{
    public class SelectHotKeysForm : Form
    {
        private struct HotKeyItem
        {
            public PictureBox icon;
            public Label text;
            public CheckBox ckeck;
            public TextBox edit;
        };

        private Options m_options;
        private HotKeyOptions m_newHotKeyOptions;

        private Button m_setDefaultButton;
        private Button m_okButton;
        private Button m_cancelButton;
        private HotKeyItem[] m_hotKeyItems;
        private ToolTip m_toolTip;

        private string m_invalidHotKeyToolTipText;

        public SelectHotKeysForm(Options options)
        {
            m_options = options;
            m_newHotKeyOptions = new HotKeyOptions(m_options.hotKeyOptions);
            InitializeComponents();
            UpdateStrings();
            UpdateBottomsEnabling();
            VerifyValidness();
            UpdateOptions();
        }

        private void InitializeComponents()
        {
            ClientSize = new System.Drawing.Size(420, 315);
            FormBorderStyle = FormBorderStyle.FixedDialog;
            StartPosition = FormStartPosition.CenterScreen;
            ShowInTaskbar = false;
            MaximizeBox = false;
            MinimizeBox = false;

            Resources.Help.Bind(this, Resources.Help.HotKeys);

            m_toolTip = new ToolTip();
            m_toolTip.ShowAlways = true;

            TableLayoutPanel mainTableLayoutPanel = InitFactory.Layout.Create(1, 2, 5);
            mainTableLayoutPanel.RowStyles.Add(new RowStyle(SizeType.Percent, 80F));
            mainTableLayoutPanel.RowStyles.Add(new RowStyle(SizeType.Percent, 20F));
            Controls.Add(mainTableLayoutPanel);

            TableLayoutPanel hotKeysTableLayoutPanel = InitFactory.Layout.Create(4, m_newHotKeyOptions.keys.Length, 5);
            hotKeysTableLayoutPanel.CellBorderStyle = TableLayoutPanelCellBorderStyle.Inset;
            hotKeysTableLayoutPanel.ColumnStyles.Add(new ColumnStyle(SizeType.Percent, 8F));
            hotKeysTableLayoutPanel.ColumnStyles.Add(new ColumnStyle(SizeType.Percent, 66F));
            hotKeysTableLayoutPanel.ColumnStyles.Add(new ColumnStyle(SizeType.Percent, 6F));
            hotKeysTableLayoutPanel.ColumnStyles.Add(new ColumnStyle(SizeType.Percent, 20F));
            mainTableLayoutPanel.Controls.Add(hotKeysTableLayoutPanel, 0, 0);
            
            m_hotKeyItems = new HotKeyItem[m_newHotKeyOptions.keys.Length];
            for(int i = 0; i < m_hotKeyItems.Length; i++)
            {
                HotKeyItem item = new HotKeyItem();
                item.icon = new PictureBox();
                item.icon.Location = new System.Drawing.Point(0, 0);
                item.icon.Size = new System.Drawing.Size(20, 20);
                item.icon.SizeMode = PictureBoxSizeMode.Zoom;
                hotKeysTableLayoutPanel.Controls.Add(item.icon, 0, i);

                item.text = new Label();
                item.text.Location = new System.Drawing.Point(0, 0);
                item.text.Dock = DockStyle.Fill;
                item.text.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
                hotKeysTableLayoutPanel.Controls.Add(item.text, 1, i);
                
                item.ckeck = new CheckBox();
                item.ckeck.Location = new System.Drawing.Point(0, 0);
                item.ckeck.Size = new System.Drawing.Size(20, 20);
                item.ckeck.Dock = DockStyle.Fill;
                item.ckeck.Tag = i;
                item.ckeck.Click += new EventHandler(OnCheckBoxClick);
                hotKeysTableLayoutPanel.Controls.Add(item.ckeck, 2, i);

                item.edit = new TextBox();
                item.edit.Location = new System.Drawing.Point(0, 0);
                item.edit.Dock = DockStyle.Fill;
                item.edit.ReadOnly = true;
                item.edit.Multiline = false;
                item.edit.KeyDown += new KeyEventHandler(OnTextBoxKeyDown);
                item.edit.Tag = i;
                hotKeysTableLayoutPanel.Controls.Add(item.edit, 3, i);
                
                m_hotKeyItems[i] = item;
            }

            InitializeIcons();

            TableLayoutPanel buttonsTableLayoutPanel = InitFactory.Layout.Create(4, 1);
            buttonsTableLayoutPanel.ColumnStyles.Add(new ColumnStyle(SizeType.Percent, 30F));
            buttonsTableLayoutPanel.ColumnStyles.Add(new ColumnStyle(SizeType.Percent, 20F));
            buttonsTableLayoutPanel.ColumnStyles.Add(new ColumnStyle(SizeType.Percent, 20F));
            buttonsTableLayoutPanel.ColumnStyles.Add(new ColumnStyle(SizeType.Percent, 30F));
            mainTableLayoutPanel.Controls.Add(buttonsTableLayoutPanel, 0, 1);

            m_okButton = new Button();
            m_okButton.Click += new System.EventHandler(OnButtonClick);
            buttonsTableLayoutPanel.Controls.Add(m_okButton, 1, 0);

            m_cancelButton = new Button();
            m_cancelButton.Click += new System.EventHandler(OnButtonClick);
            buttonsTableLayoutPanel.Controls.Add(m_cancelButton, 2, 0);
            
            m_setDefaultButton = new Button();
            m_setDefaultButton.AutoSize = true;
            m_setDefaultButton.Click += new System.EventHandler(OnButtonClick);
            buttonsTableLayoutPanel.Controls.Add(m_setDefaultButton, 3, 0);
        }
        
        private void InitializeIcons()
        {
            if (m_options.resultsOptions.viewMode == ResultsOptions.ViewMode.VerticalPairTable)
            {
                m_hotKeyItems[(int)HotKeyOptions.Action.CurrentDefectDelete].icon.Image = Resources.Images.Get("DeleteDefectVerticalButton");
                m_hotKeyItems[(int)HotKeyOptions.Action.CurrentDuplPairDeleteFirst].icon.Image = Resources.Images.Get("DeleteFirstVerticalButton");
                m_hotKeyItems[(int)HotKeyOptions.Action.CurrentDuplPairDeleteSecond].icon.Image = Resources.Images.Get("DeleteSecondVerticalButton");
                m_hotKeyItems[(int)HotKeyOptions.Action.CurrentDuplPairDeleteBoth].icon.Image = Resources.Images.Get("DeleteBothVerticalButton");
                m_hotKeyItems[(int)HotKeyOptions.Action.CurrentDuplPairRenameFirstToSecond].icon.Image = Resources.Images.Get("RenameFirstToSecondVerticalButton");
                m_hotKeyItems[(int)HotKeyOptions.Action.CurrentDuplPairRenameSecondToFirst].icon.Image = Resources.Images.Get("RenameSecondToFirstVerticalButton");
            }
            else if (m_options.resultsOptions.viewMode == ResultsOptions.ViewMode.HorizontalPairTable)
            {
                m_hotKeyItems[(int)HotKeyOptions.Action.CurrentDefectDelete].icon.Image = Resources.Images.Get("DeleteDefectHorizontalButton");
                m_hotKeyItems[(int)HotKeyOptions.Action.CurrentDuplPairDeleteFirst].icon.Image = Resources.Images.Get("DeleteFirstHorizontalButton");
                m_hotKeyItems[(int)HotKeyOptions.Action.CurrentDuplPairDeleteSecond].icon.Image = Resources.Images.Get("DeleteSecondHorizontalButton");
                m_hotKeyItems[(int)HotKeyOptions.Action.CurrentDuplPairDeleteBoth].icon.Image = Resources.Images.Get("DeleteBothHorizontalButton");
                m_hotKeyItems[(int)HotKeyOptions.Action.CurrentDuplPairRenameFirstToSecond].icon.Image = Resources.Images.Get("RenameFirstToSecondHorizontalButton");
                m_hotKeyItems[(int)HotKeyOptions.Action.CurrentDuplPairRenameSecondToFirst].icon.Image = Resources.Images.Get("RenameSecondToFirstHorizontalButton");
            }
            m_hotKeyItems[(int)HotKeyOptions.Action.CurrentMistake].icon.Image = Resources.Images.Get("MistakeButton");
        }

        private void UpdateStrings()
        {
            Strings s = Resources.Strings.Current;

            Text = s.MainMenu_View_HotKeysMenuItem_Text;

            m_setDefaultButton.Text = s.SetDefaultButton_Text;
            m_okButton.Text = s.OkButton_Text;
            m_cancelButton.Text = s.CancelButton_Text;

            m_invalidHotKeyToolTipText = s.SelectHotKeysForm_InvalidHotKeyToolTipText;

            m_hotKeyItems[(int)HotKeyOptions.Action.CurrentDefectDelete].text.Text = s.ResultsPreviewDefect_DeleteButton_ToolTip_Text;
            m_hotKeyItems[(int)HotKeyOptions.Action.CurrentDuplPairDeleteFirst].text.Text = s.ResultsPreviewDuplPair_DeleteFirstButton_ToolTip_Text;
            m_hotKeyItems[(int)HotKeyOptions.Action.CurrentDuplPairDeleteSecond].text.Text = s.ResultsPreviewDuplPair_DeleteSecondButton_ToolTip_Text;
            m_hotKeyItems[(int)HotKeyOptions.Action.CurrentDuplPairDeleteBoth].text.Text = s.ResultsPreviewDuplPair_DeleteBothButton_ToolTip_Text;
            m_hotKeyItems[(int)HotKeyOptions.Action.CurrentDuplPairRenameFirstToSecond].text.Text = s.ResultsPreviewDuplPair_RenameFirstToSecondButton_ToolTip_Text;
            m_hotKeyItems[(int)HotKeyOptions.Action.CurrentDuplPairRenameSecondToFirst].text.Text = s.ResultsPreviewDuplPair_RenameSecondToFirstButton_ToolTip_Text;
            m_hotKeyItems[(int)HotKeyOptions.Action.CurrentMistake].text.Text = s.ResultsPreviewDuplPair_MistakeButton_ToolTip_Text;
            m_hotKeyItems[(int)HotKeyOptions.Action.ShowNeighbours].text.Text = s.MainMenu_View_ShowNeighbourImageMenuItem_Text;
        }

        private void OnButtonClick(object sender, EventArgs e)
        {
            Button button = (Button)sender;
            
            if(button == m_setDefaultButton)
            {
                m_newHotKeyOptions = new HotKeyOptions();
                UpdateOptions();
                UpdateBottomsEnabling();
                VerifyValidness();
                return;
            }
            
            if (button == m_okButton)
            {
                m_newHotKeyOptions.CopyTo(ref m_options.hotKeyOptions);
                // для обновления подсказки в меню
                Resources.Strings.Update();
            }
            Close();
        }

        private void OnCheckBoxClick(object sender, EventArgs e)
        {
            CheckBox checkBox = (CheckBox)sender;
            int i = (int)checkBox.Tag;
            HotKeyItem item = m_hotKeyItems[i];
            if(!item.ckeck.Checked)
            {
                m_newHotKeyOptions.keys[i] = Keys.None;
            }
            else
            {
                m_newHotKeyOptions.SetDefault((HotKeyOptions.Action)i);
            }
            UpdateOptions();
            UpdateBottomsEnabling();
            VerifyValidness();
        }

        private void UpdateOptions()
        {
            for(int i = 0; i < m_hotKeyItems.Length; i++)
            {
                m_hotKeyItems[i].ckeck.Checked = m_newHotKeyOptions.keys[i] != Keys.None;
                m_hotKeyItems[i].edit.Text = m_newHotKeyOptions.keys[i].ToString().Replace(',', '+');
            }
        }

        private void UpdateBottomsEnabling()
        {
            m_okButton.Enabled = !m_newHotKeyOptions.Equals(m_options.hotKeyOptions) && m_newHotKeyOptions.Valid();
            m_setDefaultButton.Enabled = !m_newHotKeyOptions.Equals(new HotKeyOptions());
        }
        
        private void OnTextBoxKeyDown(object sender, KeyEventArgs e)
        {
            TextBox textBox = (TextBox)sender;
            int i = (int)textBox.Tag;
            HotKeyItem item = m_hotKeyItems[i];
            m_newHotKeyOptions.keys[i] = e.KeyData;
            UpdateOptions();
            UpdateBottomsEnabling();
            VerifyValidness();
        }

        private void VerifyValidness()
        {
            for (int i = 0; i < m_hotKeyItems.Length; i++)
            {
                if (m_newHotKeyOptions.Valid((HotKeyOptions.Action)i))
                {
                    m_toolTip.SetToolTip(m_hotKeyItems[i].edit, "");
                    m_hotKeyItems[i].edit.ForeColor = TextBox.DefaultForeColor;
                    m_hotKeyItems[i].edit.BackColor = TextBox.DefaultBackColor;
                }
                else
                {
                    m_toolTip.SetToolTip(m_hotKeyItems[i].edit, m_invalidHotKeyToolTipText);
                    m_hotKeyItems[i].edit.ForeColor = Color.Red;
                    m_hotKeyItems[i].edit.BackColor = TextBox.DefaultBackColor;
                }
            }
        }
    }
}
