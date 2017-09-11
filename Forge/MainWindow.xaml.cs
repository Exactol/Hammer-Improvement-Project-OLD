using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using MahApps.Metro.Controls;

namespace Forge
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : MetroWindow
    {
        public MainWindow()
        {
            InitializeComponent();
        }
    }

    class FieldTemplateClass
    {
        private string _fieldName;
        private string _filedHint;
        private string _fieldEntry;

        public string FieldName
        {
            get { return _fieldName; }
            set { _fieldName = value; }
        }

        public string FiledHint
        {
            get { return _filedHint; }
            set { _filedHint = value; }
        }

        public string FieldEntry
        {
            get { return _fieldEntry; }
            set { _fieldEntry = value; }
        }
    }
}