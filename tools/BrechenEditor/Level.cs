using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using System.Drawing;

namespace BrechenEditor
{
    public class Level
    {
        public Level()
        {
            FileName = string.Empty;
            Bricks = new List<Brick>();
        }

        public string FileName
        {
            get;
            set;
        }

        public List<Brick> Bricks
        {
            get;
            private set;
        }

        public void Reset()
        {
            Bricks.Clear();
        }

        public void Save()
        {
            using (BinaryWriter writer = new BinaryWriter(File.Create(FileName)))
            {
                writer.Write(Bricks.Count);
                foreach (Brick brick in Bricks)
                {
                    writer.Write((byte)(brick.Location.X / Grid.TileSize));
                    writer.Write((byte)(brick.Location.Y / Grid.TileSize));
                    writer.Write((byte)(brick.Width / Grid.TileSize));
                    writer.Write((byte)(brick.Height / Grid.TileSize));
                    writer.Write(brick.Color.ToArgb());
                }
            }
        }

        public void Load(string fileName, Grid grid)
        {
            FileName = fileName;
            using (BinaryReader reader = new BinaryReader(File.OpenRead(FileName)))
            {
                // Skip the brick count. 
                reader.ReadInt32();
                while (reader.PeekChar() != -1)
                {
                    int xPos = (int)reader.ReadByte() * Grid.TileSize;
                    int yPos = (int)reader.ReadByte() * Grid.TileSize;
                    int width = (int)reader.ReadByte() * Grid.TileSize;
                    int height = (int)reader.ReadByte() * Grid.TileSize;
                    Color color = Color.FromArgb(reader.ReadInt32());
                    Brick brick = new Brick(width, height, new Point(xPos, yPos), color);
                    for (int i = brick.Location.Y / Grid.TileSize; i < (brick.Location.Y + brick.Height) / Grid.TileSize; ++i)
                    {
                        for (int j = brick.Location.X / Grid.TileSize; j < (brick.Location.X + brick.Width) / Grid.TileSize; ++j)
                        {
                            grid[i * grid.Columns + j] = brick; 
                        }
                    }
                    Bricks.Add(brick);
                }
            }
        }
    }
}
