using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Drawing;

namespace BrechenEditor
{
    public class Brick
    {
        public Brick(int width, int height, Point location, Color color)
        {
            Width = width;
            Height = height;
            Location = location;
            Color = color;
        }

        public int Width
        {
            get;
            set;
        }

        public int Height
        {
            get;
            set;
        }

        public Point Location
        {
            get;
            set;
        }

        public Color Color
        {
            get;
            set;
        }
    }
}
