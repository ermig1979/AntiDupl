using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Linq;

namespace AntiDupl.NET.WPF.Control
{
    /// <summary>
    /// Текст с побуквенным diff-подсветкой (зеркало WF DataGridViewDoubleTextBoxCell:
    /// BuildDiffSegments/DrawTextWithDiff): SourceText сравнивается с DiffAgainst,
    /// отличающиеся сегменты рисуются MarkerBrush'ом, общие — обычным цветом.
    /// Контент — только Inlines; свой Text не используется (Text и Inlines в TextBlock
    /// взаимоисключающие модельные каналы — их смешение роняет text-систему WPF).
    /// </summary>
    public class DiffTextBlock : TextBlock
    {
        public static readonly DependencyProperty SourceTextProperty =
            DependencyProperty.Register(
                "SourceText",
                typeof(string),
                typeof(DiffTextBlock),
                new FrameworkPropertyMetadata(
                    null,
                    FrameworkPropertyMetadataOptions.BindsTwoWayByDefault,
                    OnInputChanged));

        /// <summary>Текст для отображения (первый файл пары).</summary>
        public string SourceText
        {
            get { return (string)GetValue(SourceTextProperty); }
            set { SetValue(SourceTextProperty, value); }
        }

        public static readonly DependencyProperty DiffAgainstProperty =
            DependencyProperty.Register(
                "DiffAgainst",
                typeof(string),
                typeof(DiffTextBlock),
                new FrameworkPropertyMetadata(
                    null,
                    FrameworkPropertyMetadataOptions.BindsTwoWayByDefault,
                    OnInputChanged));

        /// <summary>Строка, с которой идёт сравнение (второй файл пары).</summary>
        public string DiffAgainst
        {
            get { return (string)GetValue(DiffAgainstProperty); }
            set { SetValue(DiffAgainstProperty, value); }
        }

        public static readonly DependencyProperty MarkerBrushProperty =
            DependencyProperty.Register(
                "MarkerBrush",
                typeof(Brush),
                typeof(DiffTextBlock),
                new FrameworkPropertyMetadata(Brushes.Red, OnInputChanged));

        /// <summary>Цвет/кисть выделенных (отличающихся) сегментов.</summary>
        public Brush MarkerBrush
        {
            get { return (Brush)GetValue(MarkerBrushProperty); }
            set { SetValue(MarkerBrushProperty, value); }
        }

        static void OnInputChanged(DependencyObject d, DependencyPropertyChangedEventArgs e)
        {
            ((DiffTextBlock)d).RebuildInlines();
        }

        protected override void OnInitialized(EventArgs e)
        {
            base.OnInitialized(e);
            RebuildInlines();
        }

        public DiffTextBlock()
        {
            // TextBlock шлует OnMouseDoubleClick — ловим двойной клик через MouseLeftButtonDown
            AddHandler(UIElement.MouseLeftButtonDownEvent,
                new MouseButtonEventHandler(OnMouseLeftButtonDownHandler),
                true);
        }

        /// <summary>
        /// Двойной клик по оверлею переводит соседний RenameTextBox (в той же ячейке)
        /// в режим редактирования — так имя можно переименовать, хотя визуально его
        /// рисует оверлей с diff-подсветкой.
        /// </summary>
        static void OnMouseLeftButtonDownHandler(object sender, MouseButtonEventArgs e)
        {
            if (e.ClickCount != 2)
                return;
            DiffTextBlock block = sender as DiffTextBlock;
            if (block == null || block.Parent is not System.Windows.Controls.Panel panel)
                return;
            RenameTextBox box = panel.Children.OfType<RenameTextBox>().FirstOrDefault();
            if (box == null)
                return;
            box.EditMode = TextBoxMode.Edit;
            box.Focus();
        }

        void RebuildInlines()
        {
            string text = SourceText;
            string other = DiffAgainst;

            if (string.IsNullOrEmpty(text) || string.IsNullOrEmpty(other))
            {
                Inlines.Clear();
                if (!string.IsNullOrEmpty(text))
                    Inlines.Add(new Run(text) { Foreground = Foreground });
                return;
            }

            if (text == other)
            {
                Inlines.Clear();
                Inlines.Add(new Run(text) { Foreground = Foreground });
                return;
            }

            Inlines.Clear();
            foreach (DiffSegment segment in BuildDiffSegments(text, other))
            {
                if (string.IsNullOrEmpty(segment.Text))
                    continue;
                Inlines.Add(new Run(segment.Text)
                {
                    Foreground = segment.IsDifferent ? MarkerBrush : Foreground
                });
            }
        }

        struct DiffSegment
        {
            public string Text;
            public bool IsDifferent;
        }

        /// <summary>
        /// Портированный WF-алгоритм (DataGridViewDoubleTextBoxCell.BuildDiffSegments):
        /// идём по буквам; при расхождении ищем ближайшую точку, где строки снова
        /// совпадают минимум 2 буквы подряд («ресинхронизация»); интервал от расхождения
        /// до неё — различающийся сегмент. Без точки ресинхрона — до конца строки.
        /// </summary>
        static List<DiffSegment> BuildDiffSegments(string text, string otherText)
        {
            var segments = new List<DiffSegment>();
            int i = 0, j = 0;

            while (i < text.Length || j < otherText.Length)
            {
                // Указатели алгоритма не должны уходить за пределы строк — иначе
                // Substring/индексация ниже дадут неинформативный краш.
                Debug.Assert(i <= text.Length && j <= otherText.Length,
                    "diff pointers must stay within string bounds");
                if (i < text.Length && j < otherText.Length && text[i] == otherText[j])
                {
                    // Совпадающие символы
                    int start = i;
                    while (i < text.Length && j < otherText.Length && text[i] == otherText[j])
                    {
                        i++;
                        j++;
                    }
                    segments.Add(new DiffSegment { Text = text.Substring(start, i - start), IsDifferent = false });
                }
                else
                {
                    // Расхождение: ищем, где строки снова совпадут (>= 2 символа подряд)
                    int startI = i;
                    int startJ = j;

                    int syncPoint = -1;
                    int maxLookAhead = Math.Max(text.Length - i, otherText.Length - j);
                    for (int lookAhead = 1; lookAhead <= maxLookAhead && syncPoint < 0; lookAhead++)
                    {
                        for (int offsetI = 0; offsetI <= lookAhead && startI + offsetI < text.Length; offsetI++)
                        {
                            for (int offsetJ = 0; offsetJ <= lookAhead && startJ + offsetJ < otherText.Length; offsetJ++)
                            {
                                if (startI + offsetI < text.Length && startJ + offsetJ < otherText.Length)
                                {
                                    int matchLen = 0;
                                    while (startI + offsetI + matchLen < text.Length
                                        && startJ + offsetJ + matchLen < otherText.Length
                                        && text[startI + offsetI + matchLen] == otherText[startJ + offsetJ + matchLen])
                                    {
                                        matchLen++;
                                    }

                                    if (matchLen >= 2)
                                    {
                                        i = startI + offsetI;
                                        j = startJ + offsetJ;
                                        syncPoint = i;
                                        break;
                                    }
                                }
                            }
                            if (syncPoint >= 0)
                                break;
                        }
                    }

                    // Без точки ресинхрона — до конца строки
                    if (syncPoint < 0)
                    {
                        i = text.Length;
                        j = otherText.Length;
                    }

                    if (i > startI)
                        segments.Add(new DiffSegment { Text = text.Substring(startI, i - startI), IsDifferent = true });
                }
            }

            return segments;
        }
    }
}