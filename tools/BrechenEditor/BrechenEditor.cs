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
        private const int CANVAS_WIDTH = 576;
        private const int CANVAS_HEIGHT = 320;
        private Brick previewBrick = new Brick(8, 8, Point.Empty, Color.Black);
        private Image m_originalImage;
        private Pen m_gridPen = new Pen(Color.FromArgb(64, Color.Black));
        private bool m_eraserMode = false;

        public Grid Grid
        {
            get;
            private set;
        }

        public Level Level
        {
            get;
            private set;
        }

        public BrechenForm()
        {
            InitializeComponent();
            m_originalImage = (Image)brickGrid.Image.Clone();
            brickGrid.MouseUp += new MouseEventHandler(brickGrid_MouseUp);
            brickGrid.MouseMove += new MouseEventHandler(brickGrid_MouseMove);
            brickGrid.MouseDown += new MouseEventHandler(brickGrid_MouseDown);
            Level = new Level();
            Grid = new Grid(CANVAS_HEIGHT / Grid.TileSize, CANVAS_WIDTH / Grid.TileSize);
            brickWidthComboBox.SelectedIndex = 0;
            brickHeightComboBox.SelectedIndex = 0;
            colourPanel.BackColor = brickColourDialog.Color;
            paintBrickGrid();
        }

        void brickGrid_MouseDown(object sender, MouseEventArgs e)
        {
            if (m_eraserMode)
            {
                EraseBrick(e.Location);
            }
        }

        void EraseBrick(Point point)
        {
            Point clickLocation = Grid.GetGridAlignedPoint(point);

            if (clickLocation.X >= 0 && clickLocation.Y >= 0)
            {
                int clickIndex = GetClickIndex(clickLocation);
                if (clickIndex < Grid.Size)
                {
                    if (Grid[clickIndex] != null)
                    {
                        Brick brickToErase = Grid[clickIndex];
                        for (int i = brickToErase.Location.Y / Grid.TileSize; i < (brickToErase.Location.Y + brickToErase.Height) / Grid.TileSize; ++i)
                        {
                            for (int j = brickToErase.Location.X / Grid.TileSize; j < (brickToErase.Location.X + brickToErase.Width) / Grid.TileSize; ++j)
                            {
                                Grid[i * Grid.Columns + j] = null;
                            }
                        }
                        Level.Bricks.Remove(brickToErase);
                        paintBrickGrid();
                    }
                }
            }
        }

        void brickGrid_MouseMove(object sender, MouseEventArgs e)
        {
            if (e.Button == MouseButtons.Left)
            {
                if (m_eraserMode)
                {
                    EraseBrick(e.Location);
                }
                else
                {
                    // If there isn't a brick here, draw one.
                    Point clickLocation = Grid.GetGridAlignedPoint(e.Location);

                    if (brickWithinBounds(clickLocation))
                    {
                        // Is there already a brick occupying this position? If so, delete it.
                        int clickIndex = GetClickIndex(clickLocation);
                        if (clickIndex < Grid.Size)
                        {
                            // Check that the region is clear
                            bool regionClear = true;
                            for (int i = clickLocation.Y / Grid.TileSize; i < (clickLocation.Y + previewBrick.Height) / Grid.TileSize && regionClear; ++i)
                            {
                                for (int j = clickLocation.X / Grid.TileSize; j < (clickLocation.X + previewBrick.Width) / Grid.TileSize && regionClear; ++j)
                                {
                                    int index = i * Grid.Columns + j;
                                    if (index >= 0 && index < Grid.Size && Grid[index] != null)
                                    {
                                        regionClear = false;
                                        break;
                                    }
                                }
                            }
                            if (regionClear)
                            {
                                Brick brick = new Brick(previewBrick.Width, previewBrick.Height, clickLocation, previewBrick.Color);
                                for (int i = brick.Location.Y / Grid.TileSize; i < (brick.Location.Y + brick.Height) / Grid.TileSize; ++i)
                                {
                                    for (int j = brick.Location.X / Grid.TileSize; j < (brick.Location.X + brick.Width) / Grid.TileSize; ++j)
                                    {
                                         int index = i * Grid.Columns + j;
                                         if (index >= 0 && index < Grid.Size)
                                         {
                                             Grid[i * Grid.Columns + j] = brick;
                                         }
                                    }
                                }
                                Level.Bricks.Add(brick);
                                paintBrickGrid();
                            }
                        }
                    }
                }
            }
        }

        void brickGrid_MouseUp(object sender, MouseEventArgs e)
        {
            if (!m_eraserMode)
            {
                // Draw a brick here
                Point clickLocation = Grid.GetGridAlignedPoint(e.Location);

                if (brickWithinBounds(clickLocation))
                {
                    // Is there already a brick occupying this position? If so, delete it.
                    int clickIndex = GetClickIndex(clickLocation);
                    if (clickIndex < Grid.Size)
                    {
                        Brick brick = new Brick(previewBrick.Width, previewBrick.Height, clickLocation, previewBrick.Color);
                        for (int i = brick.Location.Y / Grid.TileSize; i < (brick.Location.Y + brick.Height) / Grid.TileSize; ++i)
                        {
                            for (int j = brick.Location.X / Grid.TileSize; j < (brick.Location.X + brick.Width) / Grid.TileSize; ++j)
                            {
                                int index = i * Grid.Columns + j;
                                if (index < Grid.Size)
                                {
                                    if (Grid[index] != null && Grid[index] != brick)
                                    {
                                        Level.Bricks.Remove(Grid[index]);
                                    }
                                    Grid[index] = brick;
                                }
                            }
                        }
                        Level.Bricks.Add(brick);
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
            for (int i = 1; i < Grid.Rows; i++)
            {
                Point start = new Point(0, i * Grid.TileSize);
                Point end = new Point(Grid.Columns * Grid.TileSize, i * Grid.TileSize);
                g.DrawLine(m_gridPen, start, end);
            }

            // Draw the column dividers
            for (int i = 1; i < Grid.Columns; i++)
            {
                Point start = new Point(i * Grid.TileSize, 0);
                Point end = new Point(i * Grid.TileSize, Grid.Rows * Grid.TileSize);
                g.DrawLine(m_gridPen, start, end);
            }

            // Draw the bricks
            foreach (Brick brick in Level.Bricks)
            {
                g.FillRectangle(new SolidBrush(brick.Color), brick.Location.X + 1, brick.Location.Y + 1,
                    brick.Width - 1, brick.Height - 1);
            }
            vertexCount.Text = "" + Level.Bricks.Count * 4;
            brickGrid.Invalidate();
        }

        private bool brickWithinBounds(Point location)
        {
            // Check if the entirety of this brick remains in bounds.
            // Negative values are right out.
            if (location.X < 0 || location.Y < 0)
            {
                return false;
            }
            // Too wide?
            if (location.X + previewBrick.Width > CANVAS_WIDTH)
            {
                return false;
            }
            // Too tall?
            if (location.Y + previewBrick.Height > CANVAS_HEIGHT)
            {
                return false;
            }

            return true;
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
                e.Graphics.FillRectangle(new SolidBrush(previewBrick.Color), 1, 1,
                    previewBrick.Width - 1, previewBrick.Height - 1);
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
            Reset();
        }

        private void Reset()
        {
            Level.Reset();
            Grid.Reset();
            paintBrickGrid();
            vertexCount.Text = "0";
        }

        private void saveAsToolStripMenuItem_Click(object sender, EventArgs e)
        {
            DialogResult result = saveAsFileDialog.ShowDialog();
            if (result == DialogResult.OK)
            {
                Level.FileName = saveAsFileDialog.FileName;
                saveFile();                  
            }
        }

        private void saveFile()
        {
            try
            {
                Level.Save();
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
                Reset();

                try
                {
                    Level.Load(openFileDialog.FileName, Grid);
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
            if (Level.FileName != string.Empty)
            {
                saveFile();
            }
            else
            {
                DialogResult result = saveAsFileDialog.ShowDialog();
                if (result == DialogResult.OK)
                {
                    Level.FileName = saveAsFileDialog.FileName;
                    saveFile();
                }
            }
        }

        private void toolStripMenuItem1_Click(object sender, EventArgs e)
        {
            Level.FileName = string.Empty;
            Reset();
        }

        private void eraserCheckBox_CheckedChanged(object sender, EventArgs e)
        {
            m_eraserMode = eraserCheckBox.Checked;
        }

        private int GetClickIndex(Point clickLocation)
        {
            return (clickLocation.Y / Grid.TileSize * Grid.Columns + clickLocation.X / Grid.TileSize);
        }
    }
}
