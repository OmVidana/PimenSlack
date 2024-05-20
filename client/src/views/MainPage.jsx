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


function MainPage(){
    return(
   
            <div>
            <Link to="Menu"> 
            <div className='title'>PS</div>
            </Link>
            <div className='firstColumn'>
            <img src={image1} style={{paddingRight:80, width:200, height:200}}></img>
            <img src={image2} style={{paddingRight:80, width:250, height:200}}></img>
            <img src={image3} style={{paddingRight:80, width:200, height:250}}></img>
            <img src={image4} style={{paddingRight:80, width:230}}></img>
            <img src={image5} style={{paddingRight:80, width:200}}></img>
            </div>
            <div className='secondColumn'>
            <img src={image6} style={{paddingRight:80, width:200}}></img>
            <img src={image7} style={{paddingRight:80, width:250}}></img>
            <img src={image8} style={{paddingRight:80, width:200, height:200}}></img>
            <img src={image9} style={{paddingRight:80, width:300}}></img>
            <img src={image10}style={{paddingRight:80, width:200}}></img>
            </div>
        </div>
       
    )
}

export default MainPage;