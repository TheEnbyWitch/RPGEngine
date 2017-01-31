using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.IO;

namespace RPGEngineManager
{
    public partial class MainForm : Form
    {
        string WorkingDirectory;
        string MapDirectory;

        void ProcessDirectory(string path, int index)
        {
            foreach (string file in Directory.EnumerateFileSystemEntries(path)) //.EnumerateFiles(WorkingDirectory))
            {
                this.assetListTree.Nodes[index].Nodes.Add(file.Remove(0, path.Length+1));
            }
        }
        public MainForm()
        {
            InitializeComponent();
            WorkingDirectory = (Path.GetFullPath(Directory.GetCurrentDirectory()+"/../"));
            MapDirectory = Path.GetFullPath(WorkingDirectory + "/maps/");
            this.Text = "RPGEngine Manager - " + WorkingDirectory;
            this.mapTreeView.Nodes.Add("maps");
            foreach (string map in Directory.EnumerateFiles(MapDirectory, "*.tmx"))
            {
                this.mapTreeView.Nodes[0].Nodes.Add(Path.GetFileName(map));
            }
            foreach(string dirs in Directory.EnumerateDirectories(WorkingDirectory)) //.EnumerateFiles(WorkingDirectory))
            {
                if(dirs.Remove(0,WorkingDirectory.Length) != "bin")
                this.assetListTree.Nodes.Add(dirs.Remove(0,WorkingDirectory.Length));
                ProcessDirectory(dirs, this.assetListTree.Nodes.Count-1);
            }
            foreach (string file in Directory.EnumerateFiles(WorkingDirectory)) //.EnumerateFiles(WorkingDirectory))
            {
                this.assetListTree.Nodes.Add(file.Remove(0, WorkingDirectory.Length));
            }
        }
    }
}
