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
        //INotifyPropertyChanged Members 不需更改
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

        //Private Variables 包括变量和命令的定义
        #region Private Variables
        /*The Variables are meant to be readonly as we mustnot change the address of any of them by creating new instances.
         *Problem with new istances is that since address changes the binding becomes invalid.
         *Instantiate all the variables in the constructor.
         */
        private readonly cBat bat;
        private readonly ICommand _HomeCmd;
        private readonly ICommand _HitBallCmd;
        private readonly ICommand _MoveToCmd;

        private readonly ICommand _ConectToPMACCmd;
        private readonly ICommand _SendToPMACCmd;
        #endregion

        //Private Variables 完成变量的初始化和命令函数的绑定
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

            _ConectToPMACCmd = new RelayCommand(ConectToPMAC, CanConectToPMAC);
            _SendToPMACCmd = new RelayCommand(SendToPMAC, CanSendToPMAC);

        }
        #endregion

        //Properties 用于交互的变量和命令
        #region Properties & Commands

        #region Commands


        public ICommand ConectToPMACCmd { get { return _ConectToPMACCmd; } }

        public ICommand SendToPMACCmd { get { return _SendToPMACCmd; } }

        public ICommand MoveToCmd { get { return _MoveToCmd; } }

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

        public string PMAC_msg
        {
            get { return bat.pmac_msg; }
            set
            {
                OnPropertyChanged("PMAC_msg");
            }
        }
        public string PMAC_cmd
        {
            get { return bat.pmac_card.m_PMAC_cmd; }
            set
            {
                bat.pmac_card.m_PMAC_cmd = value;
                OnPropertyChanged("PMAC_cmd");
            }
        }
        #endregion
        #endregion

        //Commands 命令函数的定义
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
        public bool CanConectToPMAC(object obj)
        {
            //Enable the Button in some situation
            return true;
        }
        public void ConectToPMAC(object obj)
        {
            bat.pmac_card.ConectToPMAC();
            PMAC_msg = bat.pmac_msg;
        }
        //----------------------------------------------------------
        public bool CanSendToPMAC(object obj)
        {
            //Enable the Button in some situation
            return true;
        }
        public void SendToPMAC(object obj)
        {
            bat.pmac_card.m_PMAC_cmd = PMAC_cmd;
            bat.pmac_card.SendCMD();
            PMAC_msg = bat.pmac_msg;
            PMAC_cmd = "";
        }
        #endregion
    }
}
