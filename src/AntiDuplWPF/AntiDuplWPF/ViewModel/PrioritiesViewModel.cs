using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Data;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Input;
using AntiDuplWPF.Command;
using AntiDuplWPF.Model;
using AntiDuplWPF.ObjectModel;

namespace AntiDuplWPF.ViewModel
{
    public class PrioritiesViewModel : PropertyChangedBase
    {
        public ObservableCollection<Criterion> Criterions { get; set; }
        List<Criterion> _workCriterions;
        public decimal[,] priorities;
        //public decimal[,] Priorities { get; set; }
        DataView _dataView;
        public DataView DataView
        {
            get
            {
                return _dataView;
            }
        }

        int iteration = 0;
        int jiteration = 0;

        public PrioritiesViewModel(IConfigurationModel configuration)
        {
            /*Criterions = new ObservableCollection<Criterion>();
            Criterions.Add(new Criterion() { Name = "Разрешение" });
            Criterions.Add(new Criterion() { Name = "Размер файла" });
            Criterions.Add(new Criterion() { Name = "Блочность" });
            Criterions.Add(new Criterion() { Name = "Резкость" });*/
            Criterions = configuration.Criterions;
            FillingMode = false;
        }

        private void FillDataView()
        {
            var rows = priorities.GetLength(0);
            var columns = priorities.GetLength(1);
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
                    string text = priorities[r, c].ToString();
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
                    this.priorities = new decimal[_workCriterions.Count() + 1, _workCriterions.Count() + 1];
                    FillDataView();

                    iteration = 0; //а если не стоит галочка?
                    jiteration = 0;
                    if (_workCriterions.Count > 1)
                        jiteration++;
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
                    this.priorities[iteration, jiteration] = HowMuch;
                    this.priorities[jiteration, iteration] = 1 / HowMuch;
                    FillDataView();

                    jiteration++;
                    if (jiteration == _workCriterions.Count) //переходим на следующую строчку
                    {
                        iteration++;
                        jiteration = iteration + 1;
                    }
                    if (iteration != _workCriterions.Count && jiteration < _workCriterions.Count)
                    {
                        nextCompare();
                    }
                    else
                    {
                        //HowMuchBTN.Enabled = HowMuchNUD.Enabled = false;
                        FillingMode = false;
                        //NextStepBTN.Enabled = true;
                        HowMuchText = "Приоритеты для всех критериев введены.";
                        if (jiteration == _workCriterions.Count)
                        {
                            iteration++;
                            jiteration = iteration;
                        }
                        for (int i = 0; i < _workCriterions.Count; i++)
                        {
                            this.priorities[i, i] = 1;
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
                            priorities[i, _workCriterions.Count] = 0;
                            for (int j = 0; j < _workCriterions.Count; j++)
                            {
                                //PrioritiesTableDGV.Rows[i].Cells["sumstr"].Value = 
                                //    (Convert.ToDecimal(PrioritiesTableDGV.Rows[i].Cells["sumstr"].Value.ToString()) 
                                //    + Convert.ToDecimal(PrioritiesTableDGV[j, i].Value.ToString())).ToString();

                                priorities[i, _workCriterions.Count] = priorities[i, _workCriterions.Count] + priorities[i, j];
                            }
                            priorities[_workCriterions.Count, _workCriterions.Count] = priorities[_workCriterions.Count, _workCriterions.Count]
                                + priorities[i, _workCriterions.Count];
                            /*PrioritiesTableDGV.Rows[PrioritiesTableDGV.Rows.Count - 1].Cells[PrioritiesTableDGV.Columns.Count - 1].Value 
                                = (Convert.ToDecimal(PrioritiesTableDGV.Rows[PrioritiesTableDGV.Rows.Count - 1].Cells[PrioritiesTableDGV.Columns.Count - 1].Value.ToString()) 
                                + Convert.ToDecimal(PrioritiesTableDGV.Rows[i].Cells["sumstr"].Value.ToString())).ToString();*/
                        }

                        decimal sum = priorities[_workCriterions.Count, _workCriterions.Count];
                        //Convert.ToDecimal(PrioritiesTableDGV.Rows[PrioritiesTableDGV.Rows.Count - 1].Cells[PrioritiesTableDGV.Columns.Count - 1].Value.ToString());
                        for (int i = 0; i < _workCriterions.Count; i++)
                        {
                            //this.CriterionsWeights[i] = Convert.ToDecimal(PrioritiesTableDGV.Rows[i].Cells["sumstr"].Value.ToString()) / sum;
                            _workCriterions[i].Weight = priorities[i, _workCriterions.Count] / sum;
                            //PrioritiesTableDGV.Rows[i].Cells["sumstr"].Value = this.CriterionsWeights[i].ToString();
                            priorities[i, _workCriterions.Count] = _workCriterions[i].Weight;
                        }
                        //PrioritiesTableDGV.Rows[PrioritiesTableDGV.Rows.Count - 1].Cells[PrioritiesTableDGV.Columns.Count - 1].Value = "1";
                        priorities[_workCriterions.Count, _workCriterions.Count] = 1;
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
            HowMuchText = String.Format("Во сколько раз критерий {0} важнее {1}?", _workCriterions[iteration].Name, _workCriterions[jiteration].Name);
            //HowMuchText = "Во сколько раз критерий " + Criterions[iteration].Name + " важнее " + Criterions[jiteration].Name + "?";
        }
    }
}
