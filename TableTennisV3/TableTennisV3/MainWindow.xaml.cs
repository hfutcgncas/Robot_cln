﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;

using TableTennisV3.ToolResource;

namespace TableTennisV3
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        cTCPIPServer server;

        public MainWindow()
        {
            InitializeComponent();
            server = new cTCPIPServer();
        }

        private void Button_Click(object sender, RoutedEventArgs e)
        {
            MessageBox.Show("I'm here");
           // cTCPIPServer.StartClient();
        }
    }
}
