using System;
using System.Collections.Generic;
using System.Drawing;
using System.Drawing.Drawing2D;
using System.Windows.Forms;

using AntiDupl.NET.Core;

namespace AntiDupl.NET.WinForms
{
    /// <summary>
    /// A left-aligned text cell with parts of its text in the colour for
    /// worse values. The base cell paints it a second time in that colour,
    /// clipped to each part, so the text stays exactly where the plain cells
    /// around it would put it.
    /// </summary>
    public class DataGridViewMarkedTextBoxCell : DataGridViewTextBoxCell
    {
        private const TextFormatFlags FLAGS = TextFormatFlags.NoPrefix | TextFormatFlags.SingleLine | TextFormatFlags.NoPadding;
        // Where the base cell starts a left-aligned text. A part's clip only
        // has to end inside the spaces around it, so this needn't be exact.
        private const int TEXT_LEFT = 3;
        private const int CLIP_MARGIN = 1;
        private const string SEPARATOR = " x ";

        public CharacterRange[] marks { get; set; }

        /// <summary>
        /// The numbers of an image's GetImageSizeString that are lesser than
        /// the other image's.
        /// </summary>
        static public CharacterRange[] LesserNumbers(CoreImageInfo image, CoreImageInfo other)
        {
            List<CharacterRange> numbers = new List<CharacterRange>();
            string size = image.GetImageSizeString();
            int separator = size.IndexOf(SEPARATOR);
            if (separator > 0)
            {
                if (image.width < other.width)
                    numbers.Add(new CharacterRange(0, separator));
                if (image.height < other.height)
                    numbers.Add(new CharacterRange(separator + SEPARATOR.Length, size.Length - separator - SEPARATOR.Length));
            }
            return numbers.ToArray();
        }

        private static Rectangle PartBounds(Graphics graphics, string text, CharacterRange part, Rectangle cellBounds, Font font)
        {
            int end = TextRenderer.MeasureText(graphics, text.Substring(0, part.First + part.Length), font, Size.Empty, FLAGS).Width;
            int width = TextRenderer.MeasureText(graphics, text.Substring(part.First, part.Length), font, Size.Empty, FLAGS).Width;
            Rectangle bounds = new Rectangle(cellBounds.Left + TEXT_LEFT + end - width - CLIP_MARGIN, cellBounds.Top + 2,
                width + 2 * CLIP_MARGIN, cellBounds.Height - 4);
            bounds.Intersect(cellBounds);
            return bounds;
        }

        override protected void Paint(Graphics graphics, Rectangle clipBounds, Rectangle cellBounds,
          int rowIndex, DataGridViewElementStates cellState, Object value, Object formattedValue,
          string errorText, DataGridViewCellStyle cellStyle,
          DataGridViewAdvancedBorderStyle advancedBorderStyle, DataGridViewPaintParts paintParts)
        {
            base.Paint(graphics, clipBounds, cellBounds, rowIndex, cellState,
              value, formattedValue, errorText, cellStyle, advancedBorderStyle, paintParts);

            string text = formattedValue as string;
            if (text == null || marks == null || marks.Length == 0 || (paintParts & DataGridViewPaintParts.ContentForeground) == 0)
                return;

            DataGridViewCellStyle marked = new DataGridViewCellStyle(cellStyle);
            marked.ForeColor = ThemeColors.Worse;
            Color onSelection = ThemeColors.OnSelection(ThemeColors.Worse);
            if (!onSelection.IsEmpty)
                marked.SelectionForeColor = onSelection;

            using (Region whole = graphics.Clip)
            {
                foreach (CharacterRange part in marks)
                {
                    Rectangle bounds = PartBounds(graphics, text, part, cellBounds, cellStyle.Font);
                    graphics.SetClip(bounds, CombineMode.Intersect);
                    base.Paint(graphics, bounds, cellBounds, rowIndex, cellState, value, formattedValue, errorText, marked,
                      advancedBorderStyle, DataGridViewPaintParts.Background | DataGridViewPaintParts.SelectionBackground |
                      DataGridViewPaintParts.ContentForeground);
                    graphics.Clip = whole;
                }
            }
        }
    }
}
