using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using System.ComponentModel;
using System.Windows.Input;
using System.Windows;
using System.Collections.ObjectModel;

using TableTennisV3.PresentationLayer.Models;
using TableTennisV3.MVVM.Common;

namespace TableTennisV3.PresentationLayer.ViewModels
{
    class BatViewModel : INotifyPropertyChanged
    {
        #region INotifyPropertyChanged Members

        /// <summary>
        /// Event to which the view's controls will subscribe.
        /// This will enable them to refresh themselves when the binded property changes provided you fire this event.
        /// </summary>
        public event PropertyChangedEventHandler PropertyChanged;

        /// <summary>
        /// When property is changed call this method to fire the PropertyChanged Event
        /// </summary>
        /// <param name="propertyName"></param>
        public void OnPropertyChanged(string propertyName)
        {
            //Fire the PropertyChanged event in case somebody subscribed to it
            if (PropertyChanged != null)
                PropertyChanged(this, new PropertyChangedEventArgs(propertyName));
        }
        #endregion

        #region Private Variables
        /*The Variables are meant to be readonly as we mustnot change the address of any of them by creating new instances.
         *Problem with new istances is that since address changes the binding becomes invalid.
         *Instantiate all the variables in the constructor.
         */
        private readonly cBat bat;
        private readonly ICommand _HomeCmd;
        private readonly ICommand _HitBallCmd;
        private readonly ICommand _MoveToCmd;
        #endregion

        #region Constructors

        /// <summary>
        /// Instatiates all the readonly variables
        /// </summary>
        public BatViewModel()
        {
            bat = new cBat();
          
            _HomeCmd    = new RelayCommand(Home, CanHome);
            _HitBallCmd = new RelayCommand(HitBall, CanHitBall);
            _MoveToCmd  = new RelayCommand(MoveTo, CanMoveTo);

        }
        #endregion

        #region Properties
        public int Hit_X
        {
            get { return bat.Hit_X; }
            set
            {
                bat.Hit_X = value;
                OnPropertyChanged("Hit_X");
            }
        }
        public int Hit_Y
        {
            get { return bat.Hit_Y; }
            set
            {
                bat.Hit_Y = value;
                OnPropertyChanged("Hit_Y");
            }
        }
        public int Hit_Z
        {
            get { return bat.Hit_Z; }
            set
            {
                bat.Hit_Z = value;
                OnPropertyChanged("Hit_Z");
            }
        }

        public int Hit_S
        {
            get { return bat.Hit_S; }
            set
            {
                bat.Hit_S = value;
                OnPropertyChanged("Hit_S");
            }
        }

        public int Hit_P
        {
            get { return bat.Hit_P; }
            set
            {
                bat.Hit_P = value;
                OnPropertyChanged("Hit_P");
            }
        }
        #endregion

        #region Commands
        public bool CanHome(object obj)
        {
            //Enable the Button in some situation
            return true;
        }
        public void Home(object obj)
        {
            if (bat.Home())
                return;
        }
        //----------------------------------------------------------
        public bool CanHitBall(object obj)
        {
            //Enable the Button in some situation
            return true;
        }
        public void HitBall(object obj)
        {
            if (bat.HitBall())
                return;
        }
        //----------------------------------------------------------
        public bool CanMoveTo(object obj)
        {
            //Enable the Button in some situation
            return true;
        }
        public void MoveTo(object obj)
        {
            if (bat.MoveTo())
                return;
        }
        //----------------------------------------------------------

        #endregion
    }
}
