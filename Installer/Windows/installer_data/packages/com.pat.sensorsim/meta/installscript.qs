function Component()
{
    // default constructor
}

Component.prototype.createOperations = function()
{
    component.createOperations();

    if (systemInfo.productType === "windows") {
        component.addOperation("CreateShortcut", "@TargetDir@/SensorSim.exe", "@StartMenuDir@/SensorSim.lnk");
        component.addOperation("CreateShortcut", "@TargetDir@/SensorSim.exe", "@DesktopDir@/SensorSim.lnk");
    }
}
