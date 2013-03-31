using System.ComponentModel.Design;
using System.Runtime.InteropServices;
using Microsoft.VisualStudio.Shell;

namespace Caustic {
    [PackageRegistration(UseManagedResourcesOnly = true)]
    [ProvideService(typeof(IMatVisualizerService), ServiceName = "MatVisualizerService")]
    [InstalledProductRegistration("OpenCV Matrix Visualizer", "OpenCV Matrix Visualizer", "1.0")]
    [Guid("DD87C361-AE43-4048-B93C-64AAE7551CA1")]
    public sealed class MatVisualizerPackage : Package {
        /// <summary>
        ///     Initialization of the package; register vector visualizer service
        /// </summary>
        protected override void Initialize()
        {
            base.Initialize();

            IServiceContainer serviceContainer = this;

            serviceContainer.AddService(typeof(IMatVisualizerService), new MatVisualizerService(),
                true);
        }
    }
}