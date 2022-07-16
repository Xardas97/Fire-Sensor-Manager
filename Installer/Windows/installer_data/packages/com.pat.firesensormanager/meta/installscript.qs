function Component()
{
    // default constructor
}

Component.prototype.createOperations = function()
{
    component.createOperations();

    if (systemInfo.productType === "windows") {
        component.addOperation("CreateShortcut", "@TargetDir@/FireSensorManager.exe", "@StartMenuDir@/FireSensorManager.lnk");
        component.addOperation("CreateShortcut", "@TargetDir@/FireSensorManager.exe", "@HomeDir@/Desktop/FireSensorManager.lnk");
    }
}
