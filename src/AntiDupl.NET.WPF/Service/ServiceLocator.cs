using System;
using Microsoft.Extensions.DependencyInjection;

namespace AntiDupl.NET.WPF.Service
{
    /// <summary>
    /// Точка доступа к стандартному DI-контейнеру (IServiceProvider) для элементов,
    /// у которых нет конструкторной инъекции (Behavior, Control, отдельные Model-классы).
    /// Основная часть приложения (App → MainViewModel) использует чистую
    /// конструкторную инъекцию через IServiceProvider.
    /// </summary>
    public static class ServiceLocator
    {
        static IServiceProvider _provider;

        public static IServiceProvider Provider
        {
            get { return _provider; }
            set { _provider = value; }
        }

        public static T Resolve<T>() where T : class
        {
            return _provider.GetRequiredService<T>();
        }
    }
}