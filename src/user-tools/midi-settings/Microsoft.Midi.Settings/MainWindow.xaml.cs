﻿using Microsoft.Midi.Settings.Helpers;
using Microsoft.UI.Xaml.Media;

namespace Microsoft.Midi.Settings;

public sealed partial class MainWindow : Microsoft.UI.Xaml.Window
{
    public MainWindow()
    {
        InitializeComponent();

        AppWindow.SetIcon(Path.Combine(AppContext.BaseDirectory, "Assets/DIN_Settings.png"));
        Content = null;
        Title = "AppDisplayName".GetLocalized();

        //Content = new Microsoft.UI.Xaml.Controls.Grid(); // workaround for WinAppSDK bug http://task.ms/43347736
        //this.SystemBackdrop = new MicaBackdrop();


        // todo: save and restore these settings
        this.SetWindowSize(1500, 1000);
        this.CenterOnScreen();

    }
}
