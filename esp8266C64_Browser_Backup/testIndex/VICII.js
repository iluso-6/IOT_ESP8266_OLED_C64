 
    var canvas;
    var ctx; 
    var VICRAM;
    var RAM_SIZE=1025;
    var interval;
    var xmlhttp=new XMLHttpRequest(); 
    function printLog(msg){var log=document.getElementById('Log');log.innerHTML=log.innerHTML+msg+'<br>';log.scrollTop = log.scrollHeight}   
    document.addEventListener('DOMContentLoaded',initCanvas,false);
    var fC=0;
    var cS=0;
    setInterval(updateFPS,1000);
    setInterval(function(){cS=!cS;},500);
    function updateFPS(){
      var fT = document.getElementById('fps');
      fT.innerHTML=fC+ ' fps';
      fC=0;
    }   
    function initCanvas(){  
      printLog('initCanvas');
      canvas = document.getElementById('vicoutput');
      canvas.width=384;
      canvas.height=264;
      ctx = canvas.getContext('2d');
      ctx.font = '12px Lucida Console';   
      VICRAM = new Uint8Array(RAM_SIZE);
      refreshVICRAM();
      document.addEventListener('keypress', keypress, false);
    }
    var isAlive=true;
    var aliveTimer;
    function alive( ){
      if (!isAlive) {
        xmlhttp.abort();refreshVICRAM();
        printLog('Restarting connection');
      }
    }
    function refreshVICRAM( ){
    // based on https://developer.mozilla.org/en-US/docs/Web/API/XMLHttpRequest/Sending_and_Receiving_Binary_Data
      xmlhttp.onreadystatechange=function(){
        if (xmlhttp.readyState==4 && xmlhttp.status==200){
          isAlive=true;
          var aB=xmlhttp.response;
          if (aB){
            VICRAM = new Uint8Array(aB);
            refreshDisplay();
            setTimeout(refreshVICRAM,0);
          }
        }
      }
      var tp='POST';
      var url='/VICIIRefresh';  
      if (keyp.length>0) {tp='POST';url=url+keyp;}
      keyp='';
     xmlhttp.open(tp,url,true);
      xmlhttp.setRequestHeader('Content-type','application/octet-stream');
      xmlhttp.responseType='arraybuffer';
      xmlhttp.send();
      isAlive=false;
      clearTimeout(aliveTimer);
      aliveTimer=setTimeout(alive,3000);
     refreshDisplay();   
    }
    
    function dumpVICRAM(){
      var msg='';
      for (var i=0; i<120; i++){
        var petscii = VICRAM[i];
        if (petscii<32) petscii = petscii + 64;    
        msg=msg+String.fromCharCode(petscii);
        var col=i % 40;
        if (col==39) msg=msg+'<BR>';
      }
      printLog(msg);
    }
    
  //https://www.c64-wiki.com/index.php/Color
    var pal=['#000000','#FFFFFF','#880000','#AAFFEE','#CC44CC','#00CC55','#0000AA','#EEEE77','#DD8855','#664400','#FF7777','#333333','#777777','#AAFF66','#0088FF','#BBBBBB'];
    function refreshDisplay(){
//    "  printLog('refreshDisplay');
      var tcol=pal[VICRAM[4]];//text color
      var fcol=pal[VICRAM[5]];//frame/border color
      var bcol=pal[VICRAM[6]];//back color
      ctx.fillStyle=fcol;
      ctx.fillRect(0,0,384,264);
      ctx.fillStyle=bcol;
      ctx.fillRect(32,32,320,200);
      ctx.font='10px Lucida console';
      ctx.textBaseline='top';
      ctx.textAlign='left';
      ctx.fillStyle=tcol;
      var petscii;
      for (var col=0;col<40;col++){
        for (var row=0;row<40;row++){
          petscii=VICRAM[24+row*40+col];//char rom starts at 24 offset 
          if (petscii<32) petscii = petscii + 64;
          ctx.fillText(String.fromCharCode(petscii),32+col*8,32+row*8);
        } 
      } 
      if (cS) ctx.fillRect(32+VICRAM[1]*8,32+VICRAM[0]*8,8,8);
      fC++;  
    }
   var keyp='';
    function keypress(e) {
      var keyCode = e.keyCode;
      if (keyp.length==0) {keyp=keyp+'?';}else{keyp=keyp+'&';}
      keyp=keyp+'k='+keyCode.toString();
//    "  printLog('keyDown: '+keyCode);
    }
    ;
