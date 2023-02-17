using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Data;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Input;
using AntiDuplWPF.Command;
using AntiDuplWPF.Helper;
using AntiDuplWPF.Model;
using AntiDuplWPF.ObjectModel;

namespace AntiDuplWPF.ViewModel
{
    public class PrioritiesViewModel : PropertyChangedBase, IClosingViewModel
    {
        IConfigurationModel _configuration;
        List<Criterion> _workCriterions;
        public decimal[,] _priorities;
        //public decimal[,] Priorities { get; set; }
        IEnumerable<DuplPairViewModel> _result;
        IEnumerable<DuplicateGroup> _groups;

        DataView _dataView;
        public DataView DataView
        {
            get
            {
                return _dataView;
            }
        }

        ObservableCollection<Criterion> _criterions;
        public ObservableCollection<Criterion> Criterions
        {
            get { return _criterions; }
            set
            {
                _criterions = value;
                RaisePropertyChangedEvent("Criterions");
            }
        }

        int _iteration = 0;
        int _jiteration = 0;

        public PrioritiesViewModel(IConfigurationModel configuration, IEnumerable<DuplPairViewModel> result, 
            IEnumerable<DuplicateGroup> groups)
        {
            _configuration = configuration;
            _result = result;
            _groups = groups;

            Criterions = configuration.Criterions;
            FillingMode = false;
        }

        private void FillDataView()
        {
            var rows = _priorities.GetLength(0);
            var columns = _priorities.GetLength(1);
            var t = new DataTable();
            // Add columns with name "0", "1", "2", ...
            for (var c = 0; c < columns; c++)
            {
                t.Columns.Add(new DataColumn(c.ToString()));
            }
            // Add data to DataTable
            for (var r = 0; r < rows; r++)
            {
                var newRow = t.NewRow();
                for (var c = 0; c < columns; c++)
                {
                    string text = _priorities[r, c].ToString();
                    //string text = priorities[r, c].ToString("###.######");
                    if (text.Length > 5)
                        text = text.Substring(0, 5);
                    newRow[c] = text;
                }
                t.Rows.Add(newRow);
            }
            _dataView = t.DefaultView;
            RaisePropertyChangedEvent("DataView");
        }

        bool _fillingMode;
        public bool FillingMode
        {
            get { return _fillingMode; }
            set
            {
                _fillingMode = value;
                RaisePropertyChangedEvent("FillingMode");
            }
        }

        decimal _howMuch;
        public decimal HowMuch
        {
            get { return _howMuch; }
            set
            {
                _howMuch = value;
                if (_howMuch <= 0)
                    throw new ArgumentException("_howMuch <= 0! (_howMuch=" + _howMuch + ")");
                RaisePropertyChangedEvent("HowMuch");
            }
        }

        //int _age;
        //public int Age 
        //{
        //    get { return _age; }
        //    set { _age = value; }
        //}


        //double _howMuch;
        //public double HowMuch
        //{
        //    get { return _howMuch; }
        //    set
        //    {
        //        _howMuch = value;
        //        RaisePropertyChangedEvent("HowMuch");
        //    }
        //}

        string _howMuchText;
        public string HowMuchText
        {
            get { return _howMuchText; }
            set
            {
                _howMuchText = value;
                RaisePropertyChangedEvent("HowMuchText");
            }
        }

        ICommand _startFillingCommand;
        public ICommand StartFillingCommand
        {
            get
            {
                return _startFillingCommand ?? (_startFillingCommand = new RelayCommand(arg =>
                {
                    FillingMode = true;
                    HowMuch = 1;

                    _workCriterions = Criterions.Where(c => c.IsUse).Select(c => c).ToList();
                    if (!_workCriterions.Any())
                    {
                        System.Windows.MessageBox.Show("Выделите хотя бы один критерий.");
                        FillingMode = false;
                        return;
                    }
                    this._priorities = new decimal[_workCriterions.Count() + 1, _workCriterions.Count() + 1];
                    FillDataView();

                    _iteration = 0; //а если не стоит галочка?
                    _jiteration = 0;
                    if (_workCriterions.Count > 1)
                        _jiteration++;
                    nextCompare();
                }, arg => true));
            }
        }

        ICommand _howMuchAddedCommand;
        public ICommand HowMuchAddedCommand
        {
            get
            {
                return _howMuchAddedCommand ?? (_howMuchAddedCommand = new RelayCommand(arg =>
                {
                    this._priorities[_iteration, _jiteration] = HowMuch;
                    this._priorities[_jiteration, _iteration] = 1 / HowMuch;
                    FillDataView();

                    _jiteration++;
                    if (_jiteration == _workCriterions.Count) //переходим на следующую строчку
                    {
                        _iteration++;
                        _jiteration = _iteration + 1;
                    }
                    if (_iteration != _workCriterions.Count && _jiteration < _workCriterions.Count)
                    {
                        nextCompare();
                    }
                    else
                    {
                        //HowMuchBTN.Enabled = HowMuchNUD.Enabled = false;
                        FillingMode = false;
                        //NextStepBTN.Enabled = true;
                        HowMuchText = "Приоритеты для всех критериев введены.";
                        if (_jiteration == _workCriterions.Count)
                        {
                            _iteration++;
                            _jiteration = _iteration;
                        }
                        for (int i = 0; i < _workCriterions.Count; i++)
                        {
                            this._priorities[i, i] = 1;
                        }
                        /*for (int i = 0; i < this.Criterions.Count; i++)
                        {
                            for (int j = 0; j < this.Criterions.Count; j++)
                            {
                                PrioritiesTableDGV[j, i].Value = this.priorities[i, j].ToString();
                            }
                        }
                        PrioritiesTableDGV.Columns.Add("sumstr", "Вес");
                        PrioritiesTableDGV.Rows.Add();
                        PrioritiesTableDGV.Rows[PrioritiesTableDGV.Rows.Count - 1].Cells[PrioritiesTableDGV.Columns.Count - 2].Value = "Сумма";
                        PrioritiesTableDGV.Rows[PrioritiesTableDGV.Rows.Count - 1].Cells[PrioritiesTableDGV.Columns.Count - 1].Value = "0";*/
                        for (int i = 0; i < _workCriterions.Count; i++)
                        {
                            //PrioritiesTableDGV.Rows[i].Cells["sumstr"].Value = "0";
                            _priorities[i, _workCriterions.Count] = 0;
                            for (int j = 0; j < _workCriterions.Count; j++)
                            {
                                //PrioritiesTableDGV.Rows[i].Cells["sumstr"].Value = 
                                //    (Convert.ToDecimal(PrioritiesTableDGV.Rows[i].Cells["sumstr"].Value.ToString()) 
                                //    + Convert.ToDecimal(PrioritiesTableDGV[j, i].Value.ToString())).ToString();

                                _priorities[i, _workCriterions.Count] = _priorities[i, _workCriterions.Count] + _priorities[i, j];
                            }
                            _priorities[_workCriterions.Count, _workCriterions.Count] = _priorities[_workCriterions.Count, _workCriterions.Count]
                                + _priorities[i, _workCriterions.Count];
                            /*PrioritiesTableDGV.Rows[PrioritiesTableDGV.Rows.Count - 1].Cells[PrioritiesTableDGV.Columns.Count - 1].Value 
                                = (Convert.ToDecimal(PrioritiesTableDGV.Rows[PrioritiesTableDGV.Rows.Count - 1].Cells[PrioritiesTableDGV.Columns.Count - 1].Value.ToString()) 
                                + Convert.ToDecimal(PrioritiesTableDGV.Rows[i].Cells["sumstr"].Value.ToString())).ToString();*/
                        }

                        decimal sum = _priorities[_workCriterions.Count, _workCriterions.Count];
                        //Convert.ToDecimal(PrioritiesTableDGV.Rows[PrioritiesTableDGV.Rows.Count - 1].Cells[PrioritiesTableDGV.Columns.Count - 1].Value.ToString());
                        for (int i = 0; i < _workCriterions.Count; i++)
                        {
                            //this.CriterionsWeights[i] = Convert.ToDecimal(PrioritiesTableDGV.Rows[i].Cells["sumstr"].Value.ToString()) / sum;
                            _workCriterions[i].Weight = _priorities[i, _workCriterions.Count] / sum;
                            //PrioritiesTableDGV.Rows[i].Cells["sumstr"].Value = this.CriterionsWeights[i].ToString();
                            _priorities[i, _workCriterions.Count] = _workCriterions[i].Weight;
                        }
                        //PrioritiesTableDGV.Rows[PrioritiesTableDGV.Rows.Count - 1].Cells[PrioritiesTableDGV.Columns.Count - 1].Value = "1";
                        _priorities[_workCriterions.Count, _workCriterions.Count] = 1;
                        FillDataView();

                        foreach (var criterion in _workCriterions)
                        {
                            Criterions.First(c => c.Name == criterion.Name).Weight = criterion.Weight;
                        }
                    }

                }, arg => FillingMode == true));
            }
        }

        private void nextCompare()
        {
            HowMuchText = String.Format("Во сколько раз критерий {0} важнее {1}?", _workCriterions[_iteration].Name, _workCriterions[_jiteration].Name);
            //HowMuchText = "Во сколько раз критерий " + Criterions[iteration].Name + " важнее " + Criterions[jiteration].Name + "?";
        }

        ICommand _setDefaulCommand;
        public ICommand SetDefaulCommand
        {
            get
            {
                return _setDefaulCommand ?? (_setDefaulCommand = new RelayCommand(arg =>
                {
                    Criterions = _configuration.GetDefaultCriterions();
                    //FillDataView();
                }, arg => FillingMode == false));
            }
        }

        public void OnClosing(object sender, System.ComponentModel.CancelEventArgs e)
        {
            if (_result != null)
                foreach (var item in _result)
                {
                    SaatiHelper.CalculateIndex(item, _configuration);
                }
            if (_groups != null)
                foreach (var group in _groups)
                {
                    SaatiHelper.CalculateIndex(group.FileList, _configuration);
                }
        }
    }
}
