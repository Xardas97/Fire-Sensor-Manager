function Component()
{
    // default constructor
}

Component.prototype.createOperations = function()
{
    component.createOperations();

    if (systemInfo.productType === "windows") {
        component.addOperation("CreateShortcut", "@TargetDir@/FireSensorManager.exe", "@StartMenuDir@/Fire Sensor Manager.lnk");
        component.addOperation("CreateShortcut", "@TargetDir@/FireSensorManager.exe", "@DesktopDir@/Fire Sensor Manager.lnk");
    }
}
