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
        // For Terminal for the Buttons
        private Dictionary<Button, bool> buttonWindowFlags = new Dictionary<Button, bool>();

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

            // Check if Terminal and Shell or not for the processes
            var termwindows = new (Button button, string arg)[]
            {
                (button1, "--windows-sx-ui-button1-window"),
                (button2, "--windows-sx-ui-button2-window"),
                (button3, "--windows-sx-ui-button3-window"),
                (button4, "--windows-sx-ui-button4-window"),
                (button5, "--windows-sx-ui-button5-window"),
                (button6, "--windows-sx-ui-button6-window"),
                (button7, "--windows-sx-ui-button7-window"),
                (button8, "--windows-sx-ui-button8-window"),
                (button9, "--windows-sx-ui-button9-window"),
                (button10, "--windows-sx-ui-button10-window"),
                (button11, "--windows-sx-ui-button11-window"),
                (button12, "--windows-sx-ui-button12-window"),
                (button13, "--windows-sx-ui-button13-window"),
                (button14, "--windows-sx-ui-button14-window"),
                (button15, "--windows-sx-ui-button15-window"),
                (button16, "--windows-sx-ui-button16-window"),
            };

            // Options for Command Running Laden
            foreach (var (button, arg) in termwindows)
            {
                string result = await RunCommandAsync("sx", new string[] { arg });

                // Standard = true
                bool useWindow = true;

                if (!string.IsNullOrWhiteSpace(result))
                {
                    // Erwartet "true" oder "false"
                    bool.TryParse(result.Trim(), out useWindow);
                    useWindow = !useWindow;
                }

                buttonWindowFlags[button] = useWindow;
            }
        }

        private void HandleButtonClick(Button button, string argument)
        {
            bool useWindow = buttonWindowFlags.ContainsKey(button) && buttonWindowFlags[button];

            var psi = new ProcessStartInfo
            {
                FileName = "sx",
                Arguments = argument,
                UseShellExecute = !useWindow,
                CreateNoWindow = useWindow,
                WindowStyle = useWindow ? ProcessWindowStyle.Hidden : ProcessWindowStyle.Normal
            };

            Process.Start(psi);
        }

        private void button1_Click(object sender, EventArgs e)
        {
            HandleButtonClick(button1, "--windows-sx-ui-button1");
        }

        private void button2_Click(object sender, EventArgs e)
        {
            HandleButtonClick(button1, "--windows-sx-ui-button2");
        }

        private void button3_Click(object sender, EventArgs e)
        {
            HandleButtonClick(button1, "--windows-sx-ui-button3");
        }

        private void button4_Click(object sender, EventArgs e)
        {
            HandleButtonClick(button1, "--windows-sx-ui-button4");
        }

        private void button5_Click(object sender, EventArgs e)
        {
            HandleButtonClick(button1, "--windows-sx-ui-button5");
        }

        private void button6_Click(object sender, EventArgs e)
        {
            HandleButtonClick(button1, "--windows-sx-ui-button6");
        }

        private void button7_Click(object sender, EventArgs e)
        {
            HandleButtonClick(button1, "--windows-sx-ui-button7");
        }

        private void button8_Click(object sender, EventArgs e)
        {
            HandleButtonClick(button1, "--windows-sx-ui-button8");
        }

        private void button9_Click(object sender, EventArgs e)
        {
            HandleButtonClick(button1, "--windows-sx-ui-button9");
        }

        private void button10_Click(object sender, EventArgs e)
        {
            HandleButtonClick(button1, "--windows-sx-ui-button10");
        }

        private void button11_Click(object sender, EventArgs e)
        {
            HandleButtonClick(button1, "--windows-sx-ui-button11");
        }

        private void button12_Click(object sender, EventArgs e)
        {
            HandleButtonClick(button1, "--windows-sx-ui-button12");
        }

        private void button13_Click(object sender, EventArgs e)
        {
            HandleButtonClick(button1, "--windows-sx-ui-button13");
        }

        private void button14_Click(object sender, EventArgs e)
        {
            HandleButtonClick(button1, "--windows-sx-ui-button14");
        }

        private void button15_Click(object sender, EventArgs e)
        {
            HandleButtonClick(button1, "--windows-sx-ui-button15");
        }

        private void button16_Click(object sender, EventArgs e)
        {
            HandleButtonClick(button1, "--windows-sx-ui-button16");
        }
    }
}
