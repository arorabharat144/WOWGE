using System;
using System.IO;
using System.Windows;
using System.Windows.Controls;
using Microsoft.Win32;

namespace ModelImporterGUI
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        public MainWindow()
        {
            InitializeComponent();

            if (!File.Exists(executable))
            {
                if (MessageBox.Show("Executable not found!", "Error") == MessageBoxResult.OK)
                {
                    Application.Current.Shutdown();
                }
            }

            AcceptableExtensions.InitializeAcceptableExtensions();

            sourceTextBox.AcceptsReturn = false;
            destinationTextBox.AcceptsReturn = false;

            readTypeComboBox.Items.Add("ASCII");
            readTypeComboBox.Items.Add("Binary");
            readTypeComboBox.Items.Add("Animation");

            readTypeComboBox.SelectedItem = "Binary";
        }

        private void BrowseSource_Click(object sender, RoutedEventArgs e)
        {
            OpenFileDialog openFileDialog = new OpenFileDialog();
            if (sourceTextBox.Text == "")
            {
                openFileDialog.InitialDirectory = Environment.CurrentDirectory;
            }
            else
            {
                openFileDialog.InitialDirectory = sourceTextBox.Text;
            }
            if (openFileDialog.ShowDialog() == true)
            {
                string extension = openFileDialog.FileName.Substring(openFileDialog.FileName.IndexOf('.'));

                for (int i = 0; i < AcceptableExtensions.acceptedExtensions.Count; ++i)
                {
                    if (extension == AcceptableExtensions.acceptedExtensions[i])
                    {
                        break;
                    }

                    if (i == AcceptableExtensions.acceptedExtensions.Count - 1 && extension != AcceptableExtensions.acceptedExtensions[i])
                    {
                        MessageBox.Show("Unsupported Format!", "Error");
                        return;
                    }
                }

                source = openFileDialog.FileName;
                sourceTextBox.Text = source;

                string destinationDisplay = source.Remove(source.LastIndexOf('\\'));
                destinationTextBox.Text = destinationDisplay;
            }
        }

        private void BrowseDestination_Click(object sender, RoutedEventArgs e)
        {
            if (source != null)
            {
                OpenFileDialog openFileDialog = new OpenFileDialog();
                openFileDialog.InitialDirectory = source;

                if (openFileDialog.ShowDialog() == true)
                {
                    destination = openFileDialog.FileName;
                    destinationTextBox.Text = destination;
                }
            }
            else
            {
                MessageBox.Show("No Source Selected!", "Error");
                return;
            }
        }

        private void ReadType_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            readType = readTypeComboBox.SelectedItem.ToString();
        }

        private void Import_Click(object sender, RoutedEventArgs e)
        {
            if (source == null)
            {
                MessageBox.Show("No Source Selected", "Error");
                return;
            }

            destination = source.Remove(source.IndexOf('.'));

            if (readType == "ASCII")
            {
                readType = "-ascii";
                destination += ".txt";
            }
            else if (readType == "Binary")
            {
                readType = "-binary";
                destination += ".wow";
            }
            else if (readType == "Animation")
            {
                readType = "-animation";
            }

            string arguments = "\"" + source + "\"" + " " + "\"" + destination + "\"" + " " + readType;

            System.Diagnostics.Process process = System.Diagnostics.Process.Start(executable, arguments);

        }
        
        private string executable = "..\\..\\..\\Debug\\ModelImporter.exe";

        private string source;
        private string destination;
        private string readType;
    }
}
