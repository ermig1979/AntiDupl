namespace AntiDupl.NET.Deep.Cv;

/// <summary>
/// Фабрика верификаторов (перенос GeometricVerifierFactory, только LightGlue-ветвь —
/// SIFT-верификатор референса не переносился, DD11).
/// Модель создаётся лениво: только когда включена верификация и файл модели существует.
/// </summary>
public class GeometricVerifierFactory
{
    /// <param name="createVerifier">
    /// modelPath → верификатор (хост передаёт
    /// <c>path => new LightGlueGeometricVerifier(new SuperPointLightGluePipeline(path, logger))</c>;
    /// MinMatchScore не входит в конструкцию — это параметр запуска, передаётся в Verify).
    /// </param>
    private readonly Func<string, IGeometricVerifier> _createVerifier;

    public GeometricVerifierFactory(Func<string, IGeometricVerifier> createVerifier)
    {
        _createVerifier = createVerifier ?? throw new ArgumentNullException(nameof(createVerifier));
    }

    /// <returns>Верификатор либо null (верификация отключена / модель не найдена).</returns>
    public IGeometricVerifier? Create(string modelPath)
    {
        if (string.IsNullOrWhiteSpace(modelPath))
            return null;
        try
        {
            return _createVerifier(modelPath);
        }
        catch
        {
            return null;
        }
    }
}