using System;
using System.Collections.Generic;
using System.ComponentModel;
// Added be me
using System.Configuration;
using System.Data;
using System.Diagnostics;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Xml.Linq;

namespace SX_UI
{
    public partial class Form1 : Form
    {
        // Function to run a command and return its output
        private static async Task<string> RunCommandAsync(string command, string[] args)
        {
            string arguments = string.Join(" ", Array.ConvertAll(args, a => $"\"{a}\""));

            ProcessStartInfo psi = new ProcessStartInfo
            {
                FileName = command,
                Arguments = arguments,
                RedirectStandardOutput = true,
                RedirectStandardError = true,
                UseShellExecute = false,
                CreateNoWindow = true
            };

            using (Process process = new Process { StartInfo = psi })
            {
                process.Start();

                // Async-Ausgabe lesen
                Task<string> outputTask = process.StandardOutput.ReadToEndAsync();
                Task<string> errorTask = process.StandardError.ReadToEndAsync();

                // Prozess beenden abwarten
                await Task.Run(() => process.WaitForExit());  // <-- hier kein WaitForExitAsync nötig

                string output = await outputTask;
                string error = await errorTask;

                if (process.ExitCode == 1)
                    return "";

                if (!string.IsNullOrEmpty(error))
                    output += "\nError:\n" + error;

                return output;
            }
        }

        public Form1()
        {
            InitializeComponent();
        }

        private async void Form1_Load(object sender, EventArgs e)
        {
            // Array der Buttons und ihrer Argumente
            var buttons = new (Button button, string arg)[]
            {
                (button1, "--windows-sx-ui-button1-name"),
                (button2, "--windows-sx-ui-button2-name"),
                (button3, "--windows-sx-ui-button3-name"),
                (button4, "--windows-sx-ui-button4-name"),
                (button5, "--windows-sx-ui-button5-name"),
                (button6, "--windows-sx-ui-button6-name"),
                (button7, "--windows-sx-ui-button7-name"),
                (button8, "--windows-sx-ui-button8-name"),
                (button9, "--windows-sx-ui-button9-name"),
                (button10, "--windows-sx-ui-button10-name"),
                (button11, "--windows-sx-ui-button11-name"),
                (button12, "--windows-sx-ui-button12-name"),
                (button13, "--windows-sx-ui-button13-name"),
                (button14, "--windows-sx-ui-button14-name"),
                (button15, "--windows-sx-ui-button15-name"),
                (button16, "--windows-sx-ui-button16-name"),
            };

            // Buttons nacheinander oder parallel laden
            foreach (var (button, arg) in buttons)
            {
                string name = await RunCommandAsync("sx", new string[] { arg });
                if (!string.IsNullOrWhiteSpace(name))
                    button.Text = name;
            }
        }

        private void button1_Click(object sender, EventArgs e)
        {
            var psi = new ProcessStartInfo
            {
                FileName = "sx",
                Arguments = "--windows-sx-ui-button1",
                UseShellExecute = true
            };

            Process.Start(psi);
        }

        private void button2_Click(object sender, EventArgs e)
        {
            var psi = new ProcessStartInfo
            {
                FileName = "sx",
                Arguments = "--windows-sx-ui-button2",
                UseShellExecute = true
            };

            Process.Start(psi);
        }

        private void button3_Click(object sender, EventArgs e)
        {
            var psi = new ProcessStartInfo
            {
                FileName = "sx",
                Arguments = "--windows-sx-ui-button3",
                UseShellExecute = true
            };

            Process.Start(psi);
        }

        private void button4_Click(object sender, EventArgs e)
        {
            var psi = new ProcessStartInfo
            {
                FileName = "sx",
                Arguments = "--windows-sx-ui-button4",
                UseShellExecute = true
            };

            Process.Start(psi);
        }

        private void button5_Click(object sender, EventArgs e)
        {
            var psi = new ProcessStartInfo
            {
                FileName = "sx",
                Arguments = "--windows-sx-ui-button5",
                UseShellExecute = true
            };

            Process.Start(psi);
        }

        private void button6_Click(object sender, EventArgs e)
        {
            var psi = new ProcessStartInfo
            {
                FileName = "sx",
                Arguments = "--windows-sx-ui-button6",
                UseShellExecute = true
            };

            Process.Start(psi);
        }

        private void button7_Click(object sender, EventArgs e)
        {
            var psi = new ProcessStartInfo
            {
                FileName = "sx",
                Arguments = "--windows-sx-ui-button7",
                UseShellExecute = true
            };

            Process.Start(psi);
        }

        private void button8_Click(object sender, EventArgs e)
        {
            var psi = new ProcessStartInfo
            {
                FileName = "sx",
                Arguments = "--windows-sx-ui-button8",
                UseShellExecute = true
            };

            Process.Start(psi);
        }

        private void button9_Click(object sender, EventArgs e)
        {
            var psi = new ProcessStartInfo
            {
                FileName = "sx",
                Arguments = "--windows-sx-ui-button9",
                UseShellExecute = true
            };

            Process.Start(psi);
        }

        private void button10_Click(object sender, EventArgs e)
        {
            var psi = new ProcessStartInfo
            {
                FileName = "sx",
                Arguments = "--windows-sx-ui-button10",
                UseShellExecute = true
            };

            Process.Start(psi);
        }

        private void button11_Click(object sender, EventArgs e)
        {
            var psi = new ProcessStartInfo
            {
                FileName = "sx",
                Arguments = "--windows-sx-ui-button11",
                UseShellExecute = true
            };

            Process.Start(psi);
        }

        private void button12_Click(object sender, EventArgs e)
        {
            var psi = new ProcessStartInfo
            {
                FileName = "sx",
                Arguments = "--windows-sx-ui-button12",
                UseShellExecute = true
            };

            Process.Start(psi);
        }

        private void button13_Click(object sender, EventArgs e)
        {
            var psi = new ProcessStartInfo
            {
                FileName = "sx",
                Arguments = "--windows-sx-ui-button13",
                UseShellExecute = true
            };

            Process.Start(psi);
        }

        private void button14_Click(object sender, EventArgs e)
        {
            var psi = new ProcessStartInfo
            {
                FileName = "sx",
                Arguments = "--windows-sx-ui-button14",
                UseShellExecute = true
            };

            Process.Start(psi);
        }

        private void button15_Click(object sender, EventArgs e)
        {
            var psi = new ProcessStartInfo
            {
                FileName = "sx",
                Arguments = "--windows-sx-ui-button15",
                UseShellExecute = true
            };

            Process.Start(psi);
        }

        private void button16_Click(object sender, EventArgs e)
        {
            var psi = new ProcessStartInfo
            {
                FileName = "sx",
                Arguments = "--windows-sx-ui-button16",
                UseShellExecute = true
            };

            Process.Start(psi);
        }
    }
}
