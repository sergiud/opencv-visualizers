using Caustic.Properties;
using System;
using System.Drawing;
using System.Windows.Forms;

namespace Caustic {

    public partial class PreviewForm : Form {

        public PreviewForm()
        {
            InitializeComponent();
        }

        public Image Image
        {
            set
            {
                pictureBox.Image = value;

                if (value != null) {
                    ClientSize = new Size(value.Width, value.Height + toolStrip1.Height + statusStrip1.Height);
                    Text = string.Format(Resources.FormTitleFormat, value.Width, value.Height);
                }
            }
        }

        private void saveToolStripButton_Click(object sender, EventArgs e)
        {
            if (saveFileDialog1.ShowDialog() == DialogResult.OK)
                pictureBox.Image.Save(saveFileDialog1.FileName);
        }

        private void copyToolStripButton_Click(object sender, EventArgs e)
        {
            Clipboard.SetImage(pictureBox.Image);
        }
    }
}