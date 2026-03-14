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
        public Form1()
        {
            InitializeComponent();
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            // Load the Button Names

            string name = "";

            name = ConfigurationManager.AppSettings["button1-name"];
            button1.Text = string.IsNullOrWhiteSpace(name) ? button1.Text : name;

            name = ConfigurationManager.AppSettings["button2-name"];
            button2.Text = string.IsNullOrWhiteSpace(name) ? button2.Text : name;

            name = ConfigurationManager.AppSettings["button3-name"];
            button3.Text = string.IsNullOrWhiteSpace(name) ? button3.Text : name;

            name = ConfigurationManager.AppSettings["button4-name"];
            button4.Text = string.IsNullOrWhiteSpace(name) ? button4.Text : name;

            name = ConfigurationManager.AppSettings["button5-name"];
            button5.Text = string.IsNullOrWhiteSpace(name) ? button5.Text : name;

            name = ConfigurationManager.AppSettings["button6-name"];
            button6.Text = string.IsNullOrWhiteSpace(name) ? button6.Text : name;

            name = ConfigurationManager.AppSettings["button7-name"];
            button7.Text = string.IsNullOrWhiteSpace(name) ? button7.Text : name;

            name = ConfigurationManager.AppSettings["button8-name"];
            button8.Text = string.IsNullOrWhiteSpace(name) ? button8.Text : name;

            name = ConfigurationManager.AppSettings["button9-name"];
            button9.Text = string.IsNullOrWhiteSpace(name) ? button9.Text : name;

            name = ConfigurationManager.AppSettings["button10-name"];
            button10.Text = string.IsNullOrWhiteSpace(name) ? button10.Text : name;

            name = ConfigurationManager.AppSettings["button11-name"];
            button11.Text = string.IsNullOrWhiteSpace(name) ? button11.Text : name;

            name = ConfigurationManager.AppSettings["button12-name"];
            button12.Text = string.IsNullOrWhiteSpace(name) ? button12.Text : name;

            name = ConfigurationManager.AppSettings["button13-name"];
            button13.Text = string.IsNullOrWhiteSpace(name) ? button13.Text : name;

            name = ConfigurationManager.AppSettings["button14-name"];
            button14.Text = string.IsNullOrWhiteSpace(name) ? button14.Text : name;

            name = ConfigurationManager.AppSettings["button15-name"];
            button15.Text = string.IsNullOrWhiteSpace(name) ? button15.Text : name;

            name = ConfigurationManager.AppSettings["button16-name"];
            button16.Text = string.IsNullOrWhiteSpace(name) ? button16.Text : name;
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
