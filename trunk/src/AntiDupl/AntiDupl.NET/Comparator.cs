// From DupliFinder

using System;

namespace AntiDupl.NET
{
    class Comparator
    {
        public static float Similarity(byte[] first, byte[] second)
        {
            float distance = Distance(first, second);

            int length = first.Length > second.Length ? second.Length : first.Length;

            // Need to convert this to a scale from 0 to 100
            int maxunsimilarity = (int)Math.Pow(255, 2) * length / length;
            return 100 * (1 - (distance / maxunsimilarity));
        }

        private static float Distance(byte[] first, byte[] second)
        {
            int sum = 0;

            // We'll use which ever array is shorter.
            int length = first.Length > second.Length ? second.Length : first.Length;
            for (int x = 0; x < length; x++)
            {
                sum = sum + (int)Math.Pow((first[x] - second[x]), 2);
            }

            return sum / length;
        }
    }
}
