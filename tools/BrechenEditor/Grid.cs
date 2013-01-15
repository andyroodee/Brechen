using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Drawing;

namespace BrechenEditor
{
    public class Grid
    {
        public static int TileSize
        {
            get
            {
                return 8;
            }
        }

        public Grid(int rows, int cols)
        {
            Rows = rows;
            Columns = cols;
            bricks = new Brick[Rows * Columns];
        }

        public int Rows
        {
            get;
            private set;
        }

        public int Columns
        {
            get;
            private set;
        }

        public Brick this[int i]
        {
            get
            {
                return bricks[i];
            }
            set
            {
                bricks[i] = value;
            }
        }

        public int Size
        {
            get
            {
                return Rows * Columns;
            }
        }

        public static Point GetGridAlignedPoint(Point location)
        {
           return new Point(
                location.X & -Grid.TileSize,
                location.Y & -Grid.TileSize);
        }

        public void Reset()
        {
            bricks = new Brick[Rows * Columns];
        }

        private Brick[] bricks = null;
    }
}
