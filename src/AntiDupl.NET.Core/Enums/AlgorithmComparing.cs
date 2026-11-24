namespace AntiDupl.NET.Core.Enums
{
    public enum AlgorithmComparing : int
    {
        SquaredSum = 0,
        SSIM = 1,
        Dinov2 = 2,
        /// <summary>The worst 4 × 4 block of 64 × 64 colour thumbnails (core: AD_COMPARING_LOCAL_COLOR).</summary>
        LocalColor = 3,
        /// <summary>Butteraugli's score on the pairs local colour finds within 10 % (core: AD_COMPARING_BUTTERAUGLI).</summary>
        Butteraugli = 4,
        /// <summary>The same, and what a keypoint index finds, scored on the overlap once aligned: crops, new canvases, shifted or rescaled copies (core: AD_COMPARING_BUTTERAUGLI_ALIGNED).</summary>
        ButteraugliAligned = 5,
    };
}