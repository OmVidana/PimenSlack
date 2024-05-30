import React from 'react';
import 'bootstrap/dist/css/bootstrap.css';
import '../styles/MainPage.css';
import image1 from '../Images/PimenSlack/pimiento1.png'
import image2 from '../Images/PimenSlack/pimiento2.png'
import image3 from '../Images/PimenSlack/pimiento3.png'
import image4 from '../Images/PimenSlack/pimiento4.png'
import image5 from '../Images/PimenSlack/pimiento5.png'
import image6 from '../Images/PimenSlack/pimiento6.png'
import image7 from '../Images/PimenSlack/pimiento7.png'
import image8 from '../Images/PimenSlack/pimiento8.png'
import image9 from '../Images/PimenSlack/pimiento9.png'
import image10 from '../Images/PimenSlack/pimiento10.png'
import { Link } from 'react-router-dom';
import logo from '../Logo.png'
import LogoGreen from "../LogoGreen.png";
import LogoOrange from "../LogoOrange.png";
import LogoYellow from "../LogoYellow.png";

function MainPage(){
    return(
   
            <div>
            <Link to="Menu"> 
            <h1 className='title'>PS</h1>
            </Link>
            <div className='chiles'>
            <img className = "logo" src={logo}></img>
            <img className = "logo" src={LogoGreen}></img>
            <img className = "logo" src={LogoOrange}></img>
            <img className = "logo" src={LogoYellow}></img>
            </div>
        </div>
        
       
    )
}

export default MainPage;