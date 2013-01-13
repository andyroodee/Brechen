using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.IO;

namespace BrechenEditor
{
    public partial class BrechenForm : Form
    {
        private int m_rows = 0;
        private int m_cols = 0;
        private const int TILE_SIZE = 8;
        private const int CANVAS_WIDTH = 576;
        private const int CANVAS_HEIGHT = 320;
        private Brick previewBrick = new Brick(8, 8, Point.Empty, Color.Black);
        private List<Brick> bricks = new List<Brick>();
        private Image m_originalImage;
        private Pen m_gridPen = new Pen(Color.FromArgb(64, Color.Black));
        private string m_fileName = string.Empty;
        private Brick[] m_gridPositions = null;

        public BrechenForm()
        {
            InitializeComponent();
            m_originalImage = (Image)brickGrid.Image.Clone();
            brickGrid.MouseUp += new MouseEventHandler(brickGrid_MouseUp);
            brickGrid.MouseMove += new MouseEventHandler(brickGrid_MouseMove);
            m_rows = CANVAS_HEIGHT / TILE_SIZE;
            m_cols = CANVAS_WIDTH / TILE_SIZE;
            m_gridPositions = new Brick[m_rows * m_cols];
            brickWidthComboBox.SelectedIndex = 0;
            brickHeightComboBox.SelectedIndex = 0;
            colourPanel.BackColor = brickColourDialog.Color;
            paintBrickGrid();
        }

        void brickGrid_MouseMove(object sender, MouseEventArgs e)
        {
            if (e.Button == MouseButtons.Left)
            {
                // If there isn't a brick here, draw one.
                Point clickLocation = e.Location;
                // Align to nearest grid location.
                clickLocation.X = clickLocation.X & -TILE_SIZE;
                clickLocation.Y = clickLocation.Y & -TILE_SIZE;
                // Is there already a brick occupying this position? If so, delete it.
                int clickIndex = clickLocation.Y / TILE_SIZE * m_cols + clickLocation.X / TILE_SIZE;
                if (clickIndex < m_gridPositions.Length)
                {
                    // Check that the region is clear
                    bool regionClear = true;
                    for (int i = clickLocation.Y / TILE_SIZE; i < (clickLocation.Y + previewBrick.Height) / TILE_SIZE && regionClear; ++i)
                    {
                        for (int j = clickLocation.X / TILE_SIZE; j < (clickLocation.X + previewBrick.Width) / TILE_SIZE && regionClear; ++j)
                        {
                            if (m_gridPositions[i * m_cols + j] != null)
                            {
                                regionClear = false;
                                break;
                            }
                        }
                    }
                    if (regionClear)
                    {
                        Brick brick = new Brick(previewBrick.Width, previewBrick.Height, clickLocation, previewBrick.Color);
                        for (int i = brick.Location.Y / TILE_SIZE; i < (brick.Location.Y + brick.Height) / TILE_SIZE; ++i)
                        {
                            for (int j = brick.Location.X / TILE_SIZE; j < (brick.Location.X + brick.Width) / TILE_SIZE; ++j)
                            {
                                m_gridPositions[i * m_cols + j] = brick;
                            }
                        }
                        bricks.Add(brick);
                        paintBrickGrid();
                    }
                }
            }
        }

        void paintBrickGrid()
        {
            // Draw an 8x8 grid.
            brickGrid.Image = (Image)m_originalImage.Clone();
            Image image = brickGrid.Image;
            Graphics g = Graphics.FromImage(image);

            // Draw the row dividers
            for (int i = 1; i < m_rows; i++)
            {
                Point start = new Point(0, i * TILE_SIZE);
                Point end = new Point(m_cols * TILE_SIZE, i * TILE_SIZE);
                g.DrawLine(m_gridPen, start, end);
            }

            // Draw the column dividers
            for (int i = 1; i < m_cols; i++)
            {
                Point start = new Point(i * TILE_SIZE, 0);
                Point end = new Point(i * TILE_SIZE, m_rows * TILE_SIZE);
                g.DrawLine(m_gridPen, start, end);
            }

            // Draw the bricks
            foreach (Brick brick in bricks)
            {
                g.FillRectangle(new SolidBrush(brick.Color), brick.Location.X, brick.Location.Y,
                    brick.Width, brick.Height);
            }
            brickGrid.Invalidate();
        }

        void brickGrid_MouseUp(object sender, MouseEventArgs e)
        {
            // Draw a brick here
            Point clickLocation = e.Location;
            // Align to nearest grid location.
            clickLocation.X = clickLocation.X & -TILE_SIZE;
            clickLocation.Y = clickLocation.Y & -TILE_SIZE;
            // Is there already a brick occupying this position? If so, delete it.
            int clickIndex = clickLocation.Y / TILE_SIZE * m_cols + clickLocation.X / TILE_SIZE;
            if (clickIndex < m_gridPositions.Length)
            {
                Brick brick = new Brick(previewBrick.Width, previewBrick.Height, clickLocation, previewBrick.Color);
                for (int i = brick.Location.Y / TILE_SIZE; i < (brick.Location.Y + brick.Height) / TILE_SIZE; ++i)
                {
                    for (int j = brick.Location.X / TILE_SIZE; j < (brick.Location.X + brick.Width) / TILE_SIZE; ++j)
                    {
                        int index = i * m_cols + j;
                        if (m_gridPositions[index] != null && m_gridPositions[index] != brick)
                        {
                            bricks.Remove(m_gridPositions[index]);
                        }
                        m_gridPositions[index] = brick;
                    }
                }
                bricks.Add(brick);
                paintBrickGrid();
            }
        }

        private void exitToolStripMenuItem_Click(object sender, EventArgs e)
        {
            Application.Exit();
        }

        private void button1_Click(object sender, EventArgs e)
        {
            DialogResult result = brickColourDialog.ShowDialog();
            if (result == DialogResult.OK)
            {
                colourPanel.BackColor = brickColourDialog.Color;
                previewBrick.Color = colourPanel.BackColor;
                brickPreview.Invalidate();
            }
        }

        private void brickPreview_Paint(object sender, PaintEventArgs e)
        {
            if (brickWidthComboBox.SelectedItem != null && brickHeightComboBox.SelectedItem != null)
            {
                e.Graphics.FillRectangle(new SolidBrush(previewBrick.Color), 0, 0,
                    previewBrick.Width, previewBrick.Height);
            }
        }

        private void brickWidthComboBox_SelectedIndexChanged(object sender, EventArgs e)
        {
            previewBrick.Width = int.Parse(brickWidthComboBox.SelectedItem.ToString());
            brickPreview.Invalidate();
        }

        private void brickHeightComboBox_SelectedIndexChanged(object sender, EventArgs e)
        {
            previewBrick.Height = int.Parse(brickHeightComboBox.SelectedItem.ToString());
            brickPreview.Invalidate();
        }

        private void clearToolStripMenuItem_Click(object sender, EventArgs e)
        {
            resetGrid();
        }

        private void resetGrid()
        {
            m_gridPositions = new Brick[m_rows * m_cols];
            bricks.Clear();
            paintBrickGrid();
        }

        private void saveAsToolStripMenuItem_Click(object sender, EventArgs e)
        {
            DialogResult result = saveAsFileDialog.ShowDialog();
            if (result == DialogResult.OK)
            {
                m_fileName = saveAsFileDialog.FileName;
                saveFile();                  
            }
        }

        private void saveFile()
        {
            try
            {
                using (BinaryWriter writer = new BinaryWriter(File.Create(m_fileName)))
                {
                    foreach (Brick brick in bricks)
                    {
                        writer.Write((byte)(brick.Location.X / TILE_SIZE));
                        writer.Write((byte)(brick.Location.Y / TILE_SIZE));
                        writer.Write((byte)(brick.Width / TILE_SIZE));
                        writer.Write((byte)(brick.Height / TILE_SIZE));
                        writer.Write(brick.Color.ToArgb());
                    }
                }
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message);
            }
        }

        private void openToolStripMenuItem_Click(object sender, EventArgs e)
        {
            DialogResult result = openFileDialog.ShowDialog();
            if (result == DialogResult.OK)
            {
                resetGrid();

                try
                {
                    m_fileName = openFileDialog.FileName;
                    using (BinaryReader reader = new BinaryReader(File.OpenRead(m_fileName)))
                    {
                        while (reader.PeekChar() != -1)
                        {
                            int xPos = (int)reader.ReadByte() * TILE_SIZE;
                            int yPos = (int)reader.ReadByte() * TILE_SIZE;
                            int width = (int)reader.ReadByte() * TILE_SIZE;
                            int height = (int)reader.ReadByte() * TILE_SIZE;
                            Color color = Color.FromArgb(reader.ReadInt32());
                            Brick brick = new Brick(width, height, new Point(xPos, yPos), color);
                            for (int i = brick.Location.Y / TILE_SIZE; i < brick.Height / TILE_SIZE; ++i)
                            {
                                for (int j = brick.Location.X / TILE_SIZE; j < brick.Width / TILE_SIZE; ++j)
                                {
                                    m_gridPositions[i * m_cols + j] = brick;
                                }
                            }
                            bricks.Add(brick);
                        }
                    }
                    paintBrickGrid();
                }
                catch (Exception ex)
                {
                    MessageBox.Show(ex.Message);
                }
            }
        }

        private void exitToolStripMenuItem1_Click(object sender, EventArgs e)
        {
            Application.Exit();
        }

        private void saveToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (m_fileName != string.Empty)
            {
                saveFile();
            }
            else
            {
                DialogResult result = saveAsFileDialog.ShowDialog();
                if (result == DialogResult.OK)
                {
                    m_fileName = saveAsFileDialog.FileName;
                    saveFile();
                }
            }
        }
    }
}
