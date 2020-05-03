let KEY = {
    LEFT:  37, UP:    38, RIGHT: 39, DOWN:  40
};

let COLORS = {
    LIGHT:  { road: '#6B6B6B', cityground:'#BFC0C0',  rumble: '#555555', lane: '#CCCCCC'  },
    DARK:   { road: '#696969', cityground:'#BFC0C0',  rumble: '#BBBBBB'                   },
    START:  { road: 'white',  rumble: 'white'},
    FINISH: { road: 'black', rumble: 'black'}
};


let GAMEOBJECTS = {
    BUILDING12: { x:    5, y:    5, w: 1576, h: 1382 },
    DEADTREENO1: { x: 1591, y:    5, w: 1280, h:  726 },
    DINASOURNO6: { x: 1591, y:  741, w: 1200, h:  463 },
    DINASOURNO3: { x:    5, y: 1397, w: 1130, h:  996 },
    BANNERNO1: { x: 1145, y: 1397, w: 1086, h:  748 },
    BUILDING14: { x: 2881, y:    5, w: 1024, h:  768 },
    BANNERNO4: { x: 2881, y:  783, w: 1019, h:  812 },
    COLUMN: { x:    5, y: 2403, w:  800, h: 1000 },
    BANNERNO5: { x: 2881, y: 1605, w: 1000, h:  723 },
    DINASOURNO7: { x:  815, y: 2403, w:  912, h:  876 },
    DINASOURNO1: { x: 3915, y:    5, w:  856, h:  906 },
    BUILDING6: { x: 3915, y:  921, w:  188, h:  904 },
    BUILDING9: { x:    5, y: 3413, w:  900, h:  900 },
    BUILDING2: { x: 4113, y:  921, w:  536, h:  894 },
    BUILDING16: { x: 3915, y: 1835, w:  550, h:  842 },
    BANNERNO7: { x: 3915, y: 2687, w:  822, h:  678 },
    BUILDING11: { x: 1737, y: 2403, w:  821, h:  496 },
    BANNERNO8: { x:  915, y: 3413, w:  775, h:  641 },
    DINASOURNO4: { x: 2568, y: 2403, w:  740, h:  388 },
    BANNERNO6: { x: 1700, y: 3413, w:  707, h:  407 },
    BANNERNO9: { x: 4781, y:    5, w:  673, h:  473 },
    TREENO0: { x: 4781, y:  488, w:  591, h:  545 },
    BUILDING13: { x: 4781, y: 1043, w:  517, h:  379 },
    BUILDING17: { x: 4781, y: 1432, w:  500, h:  422 },
    BANNERNO3: { x: 4781, y: 1864, w:  500, h:  188 },
    DINASOURNO2: { x: 4781, y: 2062, w:  497, h:  346 },
    BUILDING4: { x: 4781, y: 2418, w:  205, h:  479 },
    BANNERNO2: { x: 4781, y: 2907, w:  478, h:  250 },
    DINASOURNO5: { x: 4996, y: 2418, w:  445, h:  355 },
    BUILDING3: { x: 4781, y: 3167, w:  153, h:  438 },
    BUILDING15: { x: 4944, y: 3167, w:  409, h:  380 },
    TREENO1: { x: 4781, y: 3615, w:  360, h:  360 },
    TREENO2: { x: 5151, y: 3615, w:  282, h:  295 },
    DEADTREENO2: { x: 5291, y: 1432, w:  150, h:  260 },
    STUMP: { x: 4781, y: 3985, w:  195, h:  140 },
    CARNO04: { x: 5308, y: 1043, w:   78, h:  134 },
    CARNO03: { x: 5382, y:  488, w:   50, h:  113 },
    CARNO02: { x: 5382, y:  611, w:   67, h:   90 },
    POLICE_VAN: { x: 5308, y: 1187, w:   80, h:   65 },
    PLAYER_ONHILL_LEFT: { x: 5308, y: 1262, w:   80, h:   45 },
    PLAYER_ONHILL_RIGHT: { x: 5308, y: 1317, w:   80, h:   45 },
    PLAYER_ONHILL_STRAIGHT: { x: 5308, y: 1372, w:   80, h:   45 },
    PLAYER_ONGROUND_LEFT: { x: 5291, y: 1702, w:   80, h:   41 },
    PLAYER_ONGROUND_RIGHT: { x: 5291, y: 1753, w:   80, h:   41 },
    PLAYER_ONGROUND_STRAIGHT: { x: 5291, y: 1804, w:   80, h:   41 },
    CARNO01: { x: 5291, y: 1864, w:   76, h:   77 },
    TRUCK: { x: 5382, y:  711, w:   61, h:   68 }
};
    




GAMEOBJECTS.SCALE = 0.3 * (1/GAMEOBJECTS.PLAYER_ONGROUND_STRAIGHT.w);

GAMEOBJECTS.BANNERS = [GAMEOBJECTS.BANNERNO1, GAMEOBJECTS.BANNERNO2, GAMEOBJECTS.BANNERNO3, GAMEOBJECTS.BANNERNO4, GAMEOBJECTS.BANNERNO5, GAMEOBJECTS.BANNERNO6, GAMEOBJECTS.BANNERNO7, GAMEOBJECTS.BANNERNO8, GAMEOBJECTS.BANNERNO9];
GAMEOBJECTS.TREES     = [GAMEOBJECTS.TREENO1, GAMEOBJECTS.TREENO2, GAMEOBJECTS.DEADTREENO1, GAMEOBJECTS.DEADTREENO2, GAMEOBJECTS.TREENO0, GAMEOBJECTS.STUMP];
GAMEOBJECTS.BUILDINGS =  [GAMEOBJECTS.BUILDING2, GAMEOBJECTS.BUILDING3, GAMEOBJECTS.BUILDING4, GAMEOBJECTS.BUILDING6,
                    GAMEOBJECTS.BUILDING9,GAMEOBJECTS.BUILDING11, GAMEOBJECTS.BUILDING12,
                    GAMEOBJECTS.BUILDING13, GAMEOBJECTS.BUILDING14, GAMEOBJECTS.BUILDING15, GAMEOBJECTS.BUILDING16, GAMEOBJECTS.BUILDING17];
GAMEOBJECTS.CARS       = [GAMEOBJECTS.CARNO01, GAMEOBJECTS.CARNO02, GAMEOBJECTS.CARNO03, GAMEOBJECTS.CARNO04, GAMEOBJECTS.POLICE_VAN, GAMEOBJECTS.TRUCK];
GAMEOBJECTS.DINASOURS = [GAMEOBJECTS.DINASOURNO1, GAMEOBJECTS.DINASOURNO2, GAMEOBJECTS.DINASOURNO3, GAMEOBJECTS.DINASOURNO4, GAMEOBJECTS.DINASOURNO5, GAMEOBJECTS.DINASOURNO6, GAMEOBJECTS.DINASOURNO7];

let framesPerSec            = 60;                      
let frameDuration           = 1/framesPerSec;                   
let canvasWidth          = 1024;                    
let canvasHeight         = 768;
let roadSegmentsArray       = [];                      
let carsArray           = [];                      
let canvas         = document.getElementById('canvas');       
let canvasContext            = canvas.getContext('2d'); 
let backgroundImages     = null;                    
let gameObjects        = null;                    
let resolution     = canvasHeight/480;
let roadWidth      = 2200;                    
let segmentLength  = 200;                     
let rumbleLength   = 3;                       
let trackLength    = null;                    
let lanes          = 3;                       
let fieldOfView    = 120;                     
let cameraHeight   = 2800;                    
let cameraDepth    = 1 / Math.tan((fieldOfView/2) * Math.PI/180);
let drawDistance   = 300;                     
let playerX        = 0;                       
let playerZ        = (cameraHeight * cameraDepth);
let position       = 0;                       
let speed          = 0;                       
let maxSpeed       = segmentLength/frameDuration;      
let acceleration   =  maxSpeed/5;             
let breaking       = -maxSpeed;               
let decel          = -maxSpeed/5;             
let offRoadDeacceleration   = -maxSpeed/2;             
let offRoadLimit   =  maxSpeed/4;             
let totalCars      = 250;                     

let keyLeft        = false;
let keyRight       = false;
let keyFaster      = false;
let keySlower      = false;

let playerData = {
    speed: { value: null, dom: document.getElementById('speed_value')},
    points: { value: null, dom: document.getElementById('points')}
}

function increaseValue(start, increment, max) { 
    let result = start + increment;
    while (result >= max)
    result -= max;
    while (result < 0)
    result += max;
    return result;
}

function getInteger(obj, def){
    if(obj !== null) { 
        let x = parseInt(obj, 10); 
        if (!isNaN(x)) 
            return x; 
    } 
    return getInteger(def, 0); 
}

function easeIEffect(start,  end,  percentage){
    return start + (end - start)*Math.pow(percentage,2);
}

function easeIOEffect(start,  end,  percentage){
    return start + (end - start)*((-Math.cos(percentage*Math.PI)/2) + 0.5);
}

function getRandomArrElem(arr){ 
    return arr[ Math.round( (arr.length-1) * Math.random() ) ];
}

function getSpeedAfterAcc(initialSpeed, acceleration, deltat){
    let v = initialSpeed + (acceleration * deltat);
    if ( (5 * Math.round(v/500)) > 100){
        accelerating();
    }
    else{
        accelerationStops();
    }
    return v;
}

function isOverlapping(x1, w1, x2, w2, percent) {
    let half = (percent || 1)/2;
    let min1 = x1 - (w1*half);
    let max1 = x1 + (w1*half);
    let min2 = x2 - (w2*half);
    let max2 = x2 + (w2*half);
    return ! ((max1 < min2) || (min1 > max2));
}

function projectIt(obj, cameraX, cameraY, cameraZ, cameraDepth, canvasWidth, canvasHeight, roadWidth) {
    obj.camera.x     = (obj.world.x || 0) - cameraX;
    obj.camera.y     = (obj.world.y || 0) - cameraY;
    obj.camera.z     = (obj.world.z || 0) - cameraZ;
    obj.screen.scale = cameraDepth/obj.camera.z;
    obj.screen.x     = Math.round((canvasWidth/2)  + (obj.screen.scale * obj.camera.x  * canvasWidth/2));
    obj.screen.y     = Math.round((canvasHeight/2) - (obj.screen.scale * obj.camera.y  * canvasHeight/2));
    obj.screen.w     = Math.round(             (obj.screen.scale * roadWidth   * canvasWidth/2));
}

function loadImgs(names, callback) { 
    let result = [];
    let count  = names.length;

    let onload = function() {
        if (--count == 0)
            callback(result);
    };

    for(let n = 0 ; n < names.length ; n++) {
        let name = names[n];
        result[n] = document.createElement('img');
        result[n].addEventListener('load', onload);
        result[n].src = "../../static/images/" + name + ".png";
    }
}

function setKeyListener(keys) {
    let onkey = function(keyCode, mode) {
        let n, k;
        for(n = 0 ; n < keys.length ; n++) {
            k = keys[n];
            k.mode = k.mode || 'up';
            if ((k.key == keyCode) || (k.keys && (k.keys.indexOf(keyCode) >= 0))) {
                if (k.mode == mode) {
                    k.action.call();
                }
            }
        }
    };
    document.addEventListener('keydown', function(ev) { onkey(ev.keyCode, 'down'); } );
    document.addEventListener('keyup',   function(ev) { onkey(ev.keyCode, 'up');   } );
}

function run(options) {

    loadImgs(options.images, function(images) {

        options.ready(images); 

        setKeyListener(options.keys);

        let now    = null,
            last   = new Date().getTime(),
            deltat     = 0,
            gdeltat    = 0;

        function frame() {
            now = new Date().getTime();
            deltat  = Math.min(1, (now - last) / 1000); 
            gdeltat = gdeltat + deltat;
            while (gdeltat > frameDuration) {
                gdeltat = gdeltat - frameDuration;
                update(frameDuration);
            }
            render();
            last = now;
            requestAnimationFrame(frame, canvas);
        }
        frame(); 
    });
}

function renderGameObjects(canvasContext, canvasWidth, roadWidth, gameObjects, gameObject, scale, destX, destY, offsetX, offsetY, clipY) {

    let destW  = (gameObject.w * scale * canvasWidth/2) * (GAMEOBJECTS.SCALE * roadWidth);
    let destH  = (gameObject.h * scale * canvasWidth/2) * (GAMEOBJECTS.SCALE * roadWidth);

    destX = destX + (destW * (offsetX || 0));
    destY = destY + (destH * (offsetY || 0));

    let clipH = clipY ? Math.max(0, destY+destH-clipY) : 0;
    if (clipH < destH)
        canvasContext.drawImage(gameObjects, gameObject.x, gameObject.y, gameObject.w, gameObject.h - (gameObject.h*clipH/destH), destX, destY, destW, destH - clipH);

}


function renderPlayer(canvasContext, canvasWidth, roadWidth, gameObjects, speedPercent, scale, destX, destY, steer, updown) {

    let arr = [-1,1];    
    let bounce = (1.5 * Math.random() * speedPercent * resolution) * arr[Math.round(Math.random())];
    let gameObject;
    if (steer < 0)
        gameObject = (updown > 0) ? GAMEOBJECTS.PLAYER_ONHILL_LEFT : GAMEOBJECTS.PLAYER_ONGROUND_LEFT;
    else if (steer > 0)
        gameObject = (updown > 0) ? GAMEOBJECTS.PLAYER_ONHILL_RIGHT : GAMEOBJECTS.PLAYER_ONGROUND_RIGHT;
    else
        gameObject = (updown > 0) ? GAMEOBJECTS.PLAYER_ONHILL_STRAIGHT : GAMEOBJECTS.PLAYER_ONGROUND_STRAIGHT;

renderGameObjects(canvasContext, canvasWidth, roadWidth, gameObjects, gameObject, scale, destX, destY + bounce, -0.5, -1);
}

function draw_polygon(canvasContext, x1, y1, x2, y2, x3, y3, x4, y4, color) {
    canvasContext.fillStyle = color;
    canvasContext.beginPath();
    canvasContext.moveTo(x1, y1);
    canvasContext.lineTo(x2, y2);
    canvasContext.lineTo(x3, y3);
    canvasContext.lineTo(x4, y4);
    canvasContext.closePath();
    canvasContext.fill();
}

function drawSegment(canvasContext, canvasWidth, lanes, x1, y1, w1, x2, y2, w2, fog, color) {

    let r1 = w1 / Math.max(6,  2*lanes),
        r2 = w2 / Math.max(6,  2*lanes),
        l1 = w1 / Math.max(32, 8*lanes),
        l2 = w2 / Math.max(32, 8*lanes),
        lanew1, lanew2, lanex1, lanex2, lane;
    
    canvasContext.fillStyle = color.cityground;
    canvasContext.fillRect(0, y2,canvasWidth, y1 - y2);
    
    draw_polygon(canvasContext, x1-w1-r1, y1, x1-w1, y1, x2-w2, y2, x2-w2-r2, y2, color.rumble);
    draw_polygon(canvasContext, x1+w1+r1, y1, x1+w1, y1, x2+w2, y2, x2+w2+r2, y2, color.rumble);
    draw_polygon(canvasContext, x1-w1,    y1, x1+w1, y1, x2+w2, y2, x2-w2,    y2, color.road);
    
    if (color.lane) {
        lanew1 = w1*2/lanes;
        lanew2 = w2*2/lanes;
        lanex1 = x1 - w1 + lanew1;
        lanex2 = x2 - w2 + lanew2;
        for(lane = 1 ; lane < lanes ; lanex1 += lanew1, lanex2 += lanew2, lane++)
        draw_polygon(canvasContext, lanex1 - l1/2, y1, lanex1 + l1/2, y1, lanex2 + l2/2, y2, lanex2 - l2/2, y2, color.lane);
    }
}

function startgame(){
    let music = document.getElementById('music');
    music.loop = true;
    music.volume = 0.8; 
    music.muted = false;
    music.play();	

    document.getElementById('enter').style.display = 'none';
}

function collisionStart(){	
    let collisionMusic = document.getElementById('collisionMusic');
    collisionMusic.volume = .6;
    collisionMusic.play();
}
function accelerating(){
    let accMusic = document.getElementById('acceleratingMusic');
    accMusic.volume = 0.6;
    accMusic.play();
}

function accelerationStops(){
    document.getElementById('acceleratingMusic').pause();
}


function getCookie(name) {  
    let cookieValue = null;
    if (document.cookie && document.cookie !== "") {
        let cookies = document.cookie.split(";");
        
        (function() {
            let i = 0;
            for ( i = 0; i < cookies.length; i+=1) {
                let cookie = cookies[i].trim();
                
                if (cookie.substring(0, name.length + 1) === (name + "=")) {
                    cookieValue = decodeURIComponent(cookie.substring(name.length + 1));
                    break;
                }
            }
        })();        
    }
    return cookieValue;
}

function sendRequest(url, type, data){
    let request = new XMLHttpRequest();
    let csrftoken = getCookie("csrftoken");
    return new Promise(function(resolve, reject){
        request.onreadystatechange = () => {
            if (request.readyState !== 4) return;
            
            if (request.status >= 200 && request.status < 300) {
                
                resolve(request.responseText);
            } else {
                
                reject({
                    status: request.status,
                    statusText: request.statusText
                });
            }
        };
        
        request.open(type || "GET", url, true);
        
        request.setRequestHeader("X-CSRFToken", csrftoken);
        
        request.send(JSON.stringify(data));
    });
}
    
function updateLeaderboard(){
    let url = "/game/updateScores/";
    let data_to_send = {
        'score' : document.getElementById('points').innerHTML
    };
    sendRequest(url,"POST",data_to_send).then(
        function (response) {
            let objRecieved = JSON.parse(response);
            let template = document.getElementById("leaderboard-row");
            for (let i = 0; i < objRecieved.username.length; ++i) {
                let clone = template.content.cloneNode(true);
                document.getElementById('leaderboard-body').appendChild(clone);
                document.getElementsByClassName('rank')[i + 1].innerHTML = i+1; 
                document.getElementsByClassName('name')[i + 1].innerHTML = objRecieved.username[i]; 
                document.getElementsByClassName('score')[i + 1].innerHTML = objRecieved.score[i]; 
            }
            document.getElementById('userRank').innerHTML = 'Your Rank : ' + objRecieved.rank;
            document.getElementById('leaderboard').style.display = 'block';
        }
    ).catch(
        function (error) {
            console.log("Something went wrong", error);
        }
    );
}

function update(deltat) {

    let n, car, carW, gameObject, gameObjectW;
    let playerSegment = getSegment(position+playerZ);
    let playerW       = GAMEOBJECTS.PLAYER_ONGROUND_STRAIGHT.w * GAMEOBJECTS.SCALE;
    let speedPercent  = speed/maxSpeed;
    let dx            = deltat * 2 * speedPercent; 

    updateCarObjects(deltat, playerSegment, playerW);

    position = increaseValue(position, deltat * speed, trackLength);

    if (keyLeft)
        playerX = playerX - dx;
    else if (keyRight)
        playerX = playerX + dx;

    playerX = playerX - (dx * speedPercent * playerSegment.curve * 0.3);

    if (keyFaster)
        speed = getSpeedAfterAcc(speed, acceleration, deltat);
    else if (keySlower)
        speed = getSpeedAfterAcc(speed, breaking, deltat);
    else
        speed = getSpeedAfterAcc(speed, decel, deltat);


    if ((playerX < -1) || (playerX > 1)) {

        if (speed > offRoadLimit)
            speed = getSpeedAfterAcc(speed, offRoadDeacceleration, deltat);

        for(n = 0 ; n < playerSegment.gameObjects.length ; n++) {
            gameObject  = playerSegment.gameObjects[n];
            gameObjectW = gameObject.source.w * GAMEOBJECTS.SCALE;
            if (isOverlapping(playerX, playerW, gameObject.offset + gameObjectW/2 * (gameObject.offset > 0 ? 1 : -1), gameObjectW)) {
                console.log('collision outside road??');
                speed = maxSpeed/5;
                position = increaseValue(playerSegment.p1.world.z, -playerZ, trackLength); 
                break;
            }
        }
    }

    for(n = 0 ; n < playerSegment.carsArray.length ; n++) {
        car  = playerSegment.carsArray[n];
        carW = car.gameObject.w * GAMEOBJECTS.SCALE;
        if (speed > car.speed) {
            if (isOverlapping(playerX, playerW, car.offset, carW, 0)) {
                collisionStart();
                console.log('collision with car');
                let lifeBar = document.getElementById('lifeValue');
                let health = lifeBar.style.width;
                if(health === '0%'){
                    console.log('Game over!!');
                    updateLeaderboard();
                }
                else{
                    let currWidth = parseInt(lifeBar.style.width.substring(0, lifeBar.style.width.length - 1), 10);
                    console.log('currcanvasWidth = ', currWidth);
                    lifeBar.style.width = (( currWidth - 10) >= 0 ? (currWidth - 10) : 0) + '%';
                    if(lifeBar.style.width === '0%'){
                        console.log('Game over!!');
                        updateLeaderboard();
                    }
                }
                speed    = car.speed * (car.speed/speed);
                position = increaseValue(car.z, -playerZ, trackLength);
                break;
            }
        }
        if (isOverlapping(playerX, playerW, car.offset, carW, 1.5) && !isOverlapping(playerX, playerW, car.offset, carW, 0) ) {
            console.log('just touched a car');            
            console.log('yur speed is ', speed)
            if((5 * Math.round(speed/500)) > 100){
                console.log('Hurray!! you deserve 100 points!!!');
                let pointsVal = (playerData['points'].value == null) ? 100 : playerData['points'].value + 100;
                updatePlayerData('points', pointsVal);
            }
            break;
        }
    }

    playerX = Math.max(-3, Math.min(playerX, 3));    
    speed   = Math.max(0, Math.min(speed, maxSpeed)); 

    updatePlayerData('speed', 5 * Math.round(speed/500));
}



function updateCarObjects(deltat, playerSegment, playerW) {
    let n, car, oldSegment, newSegment;
    for(n = 0 ; n < carsArray.length ; n++) {
        car         = carsArray[n];
        oldSegment  = getSegment(car.z);
        car.offset  = car.offset + getCarNewOffset(car, oldSegment, playerSegment, playerW);
        car.z       = increaseValue(car.z, deltat * car.speed, trackLength);
        car.percent = (car.z %  segmentLength) /  segmentLength; 
        newSegment  = getSegment(car.z);
        if (oldSegment != newSegment) {
            index = oldSegment.carsArray.indexOf(car);
            oldSegment.carsArray.splice(index, 1);
            newSegment.carsArray.push(car);
        }
    }
}

function getCarNewOffset(carObject, carSegment, myCarSegment, myW) {

    let i, j, dir, segment, secondCar, secondCarW, lukahead = 20, carObjectW = carObject.gameObject.w * GAMEOBJECTS.SCALE;

    
    if ((carSegment.index - myCarSegment.index) > drawDistance)
        return 0;

    for(i = 1 ; i < lukahead ; i++) {
        segment = roadSegmentsArray[(carSegment.index+i)%roadSegmentsArray.length];

        if ((segment === myCarSegment) && (carObject.speed > speed) && (isOverlapping(playerX, myW, carObject.offset, carObjectW, 1.2))) {
            if (playerX > 0.5)
                dir = -1;
            else if (playerX < -0.5)
                dir = 1;
            else
                dir = (carObject.offset > playerX) ? 1 : -1;
            return dir * 1/i * (carObject.speed-speed)/maxSpeed; 
        }

        for(j = 0 ; j < segment.carsArray.length ; j++) {
            secondCar  = segment.carsArray[j];
            secondCarW = secondCar.gameObject.w * GAMEOBJECTS.SCALE;
            if ((carObject.speed > secondCar.speed) && isOverlapping(carObject.offset, carObjectW, secondCar.offset, secondCarW, 1.2)) {
                if (secondCar.offset > 0.5)
                    dir = -1;
                else if (secondCar.offset < -0.5)
                    dir = 1;
                else
                    dir = (carObject.offset > secondCar.offset) ? 1 : -1;
                return dir * 1/i * (carObject.speed-secondCar.speed)/maxSpeed;
            }
        }
    }

    
    if (carObject.offset < -0.9)
        return 0.1;
    else if (carObject.offset > 0.9)
        return -0.1;
    else
        return 0;
}



function updatePlayerData(key, value) { 
    if (playerData[key].value !== value) {
        playerData[key].value = value;
        playerData[key].dom.innerHTML = value;
    }
}

function render() {

    let baseSegment   = getSegment(position);
    let basePercent   = (position %  segmentLength) /  segmentLength;
    let playerSegment = getSegment(position+playerZ);
    let playerPercent = ((position+playerZ) %  segmentLength) /  segmentLength;
    let playerY       = ( (playerSegment.p1.world.y) + ( ( playerSegment.p2.world.y) - (playerSegment.p1.world.y) ) * (  playerPercent ) );
    let maxy          = canvasHeight;

    let x  = 0;
    let dx = - (baseSegment.curve * basePercent);

    canvasContext.clearRect(0, 0, canvasWidth, canvasHeight);
    canvasContext.drawImage(backgroundImages, 0, 0, window.innerWidth, window.innerHeight);

    let n, i, segment, car, gameObject, gameObjectScale, gameObjectX, gameObjectY;

    for(n = 0 ; n < drawDistance ; n++) {

        segment        = roadSegmentsArray[(baseSegment.index + n) % roadSegmentsArray.length];
        segment.looped = segment.index < baseSegment.index;
        segment.clip   = maxy;

        projectIt(segment.p1, (playerX * roadWidth) - x,      playerY + cameraHeight, position - (segment.looped ? trackLength : 0), cameraDepth, canvasWidth, canvasHeight, roadWidth);
        projectIt(segment.p2, (playerX * roadWidth) - x - dx, playerY + cameraHeight, position - (segment.looped ? trackLength : 0), cameraDepth, canvasWidth, canvasHeight, roadWidth);

        x  = x + dx;
        dx = dx + segment.curve;

        if ((segment.p1.camera.z <= cameraDepth)         || 
                (segment.p2.screen.y >= segment.p1.screen.y) || 
                (segment.p2.screen.y >= maxy))                  
            continue;

            drawSegment(canvasContext, canvasWidth, lanes, segment.p1.screen.x, segment.p1.screen.y, segment.p1.screen.w,
                        segment.p2.screen.x, segment.p2.screen.y, segment.p2.screen.w, segment.fog, segment.color);

        maxy = segment.p1.screen.y;
    }

    for(n = (drawDistance-1) ; n > 0 ; n--) {
        segment = roadSegmentsArray[(baseSegment.index + n) % roadSegmentsArray.length];

        for(i = 0 ; i < segment.carsArray.length ; i++) {
            car         = segment.carsArray[i];
            gameObject      = car.gameObject;
            gameObjectScale = ( (segment.p1.screen.scale) + ( ( segment.p2.screen.scale) - (segment.p1.screen.scale) ) * (  car.percent ) );
            gameObjectX     = ( (segment.p1.screen.x) + ( (     segment.p2.screen.x) - (segment.p1.screen.x) ) * (      car.percent) + (gameObjectScale * car.offset * roadWidth * canvasWidth/2 ) );
            gameObjectY     = ( (segment.p1.screen.y) + ( (     segment.p2.screen.y) - (segment.p1.screen.y) ) * (      car.percent ) );
            renderGameObjects(canvasContext, canvasWidth, roadWidth, gameObjects, car.gameObject, gameObjectScale, gameObjectX, gameObjectY, -0.5, -1, segment.clip);
        }

        for(i = 0 ; i < segment.gameObjects.length ; i++) {
            gameObject      = segment.gameObjects[i];
            gameObjectScale = segment.p1.screen.scale;
            gameObjectX     = segment.p1.screen.x + (gameObjectScale * gameObject.offset * roadWidth * canvasWidth/2);
            gameObjectY     = segment.p1.screen.y;
            renderGameObjects(canvasContext, canvasWidth, roadWidth, gameObjects, gameObject.source, gameObjectScale, gameObjectX, gameObjectY, (gameObject.offset < 0 ? -1 : 0), -1, segment.clip);
        }

        if (segment == playerSegment) {
            renderPlayer(canvasContext, canvasWidth, roadWidth, gameObjects, speed/maxSpeed,
                        cameraDepth/playerZ, canvasWidth/2, (canvasHeight/2) - (cameraDepth/playerZ * ( (playerSegment.p1.camera.y) + ( ( playerSegment.p2.camera.y) - (playerSegment.p1.camera.y) ) * (  playerPercent ) ) * canvasHeight/2),
                        speed * (keyLeft ? -1 : keyRight ? 1 : 0), playerSegment.p2.world.y - playerSegment.p1.world.y);
        }
    }
}

function getSegment(z) {
    return roadSegmentsArray[Math.floor(z/segmentLength) % roadSegmentsArray.length]; 
}

function lastY() { 
    return (roadSegmentsArray.length == 0) ? 0 : roadSegmentsArray[roadSegmentsArray.length-1].p2.world.y; 
}

function addSegment(curve, y) {
    let n = roadSegmentsArray.length;
    console.log('adding segment', curve, y);
    roadSegmentsArray.push({
            index: n,
                    p1: { world: { y: lastY(), z:  n   *(segmentLength) }, camera: {}, screen: {} },
                    p2: { world: { y: y,       z: (n+1)*segmentLength }, camera: {}, screen: {} },
            curve: curve,
        gameObjects: [],
                carsArray: [],
            color: Math.floor(n/rumbleLength)%2 ? COLORS.DARK : COLORS.LIGHT
    });
}

function addGameObject(n, gameObject, offset) {
    roadSegmentsArray[n].gameObjects.push({ source: gameObject, offset: offset });
}

function addRoadtoCanvas(enter, hold, leave, curve, y) {
    let startY   = lastY();
    let endY     = startY + (getInteger(y, 0) * segmentLength);
    let n, total = enter + hold + leave;
    for(n = 0 ; n < enter ; n++)
        addSegment(easeIEffect(0, curve, n/enter), easeIOEffect(startY, endY, n/total));
    for(n = 0 ; n < hold  ; n++)
        addSegment(curve, easeIOEffect(startY, endY, (enter+n)/total));
    for(n = 0 ; n < leave ; n++)
        addSegment(easeIOEffect(curve, 0, n/leave), easeIOEffect(startY, endY, (enter+hold+n)/total));
}

let ROAD_OBJ = {
    LEN: { NONE: 0, SHORT:  25, MED:   50, LONG:  100 },
    HILL_PART:   { NONE: 0, LOW:    20, MED:   40, HI:   60 },
    CURVE_PART:  { NONE: 0, EASY:    2, MED:    4}
};

function addStraightPortion(num) {
    num = num || ROAD_OBJ.LEN.MED;
    addRoadtoCanvas(num, num, num, 0, 0);
}

function addHillPortion(num, canvasHeight) {
    num    = num    || ROAD_OBJ.LEN.MED;
    canvasHeight = canvasHeight || ROAD_OBJ.HILL_PART.MED;
    addRoadtoCanvas(num, num, num, 0, canvasHeight);
}

function addCurvePortion(num, curve, canvasHeight) {
    num    = num    || ROAD_OBJ.LEN.MED;
    curve  = curve  || ROAD_OBJ.CURVE_PART.MED;
    canvasHeight = canvasHeight || ROAD_OBJ.HILL_PART.NONE;
    addRoadtoCanvas(num, num, num, curve, canvasHeight);
}
        
function addLowRollingHills(num, canvasHeight) {
    num    = num    || ROAD_OBJ.LEN.SHORT;
    canvasHeight = canvasHeight || ROAD_OBJ.HILL_PART.LOW;
    addRoadtoCanvas(num, num, num,  0,                canvasHeight/2);
    addRoadtoCanvas(num, num, num,  0,               -canvasHeight);
    addRoadtoCanvas(num, num, num,  ROAD_OBJ.CURVE_PART.EASY,  canvasHeight);
    addRoadtoCanvas(num, num, num,  0,                0);
    addRoadtoCanvas(num, num, num, -ROAD_OBJ.CURVE_PART.EASY,  canvasHeight/2);
    addRoadtoCanvas(num, num, num,  0,                0);
}

function addStraightCurves() {
    addRoadtoCanvas(ROAD_OBJ.LEN.MED, ROAD_OBJ.LEN.MED, ROAD_OBJ.LEN.MED,  -ROAD_OBJ.CURVE_PART.EASY,    ROAD_OBJ.HILL_PART.NONE);
    addRoadtoCanvas(ROAD_OBJ.LEN.MED, ROAD_OBJ.LEN.MED, ROAD_OBJ.LEN.MED,   ROAD_OBJ.CURVE_PART.MED,  ROAD_OBJ.HILL_PART.MED);
    addRoadtoCanvas(ROAD_OBJ.LEN.MED, ROAD_OBJ.LEN.MED, ROAD_OBJ.LEN.MED,   ROAD_OBJ.CURVE_PART.EASY,   -ROAD_OBJ.HILL_PART.LOW);
    addRoadtoCanvas(ROAD_OBJ.LEN.MED, ROAD_OBJ.LEN.MED, ROAD_OBJ.LEN.MED,  -ROAD_OBJ.CURVE_PART.EASY,    ROAD_OBJ.HILL_PART.MED);
    addRoadtoCanvas(ROAD_OBJ.LEN.MED, ROAD_OBJ.LEN.MED, ROAD_OBJ.LEN.MED,  -ROAD_OBJ.CURVE_PART.MED, -ROAD_OBJ.HILL_PART.MED);
}

function addBumpyPortions() {
    addRoadtoCanvas(10, 10, 10, 0,  5);
    addRoadtoCanvas(10, 10, 10, 0, -2);
    addRoadtoCanvas(10, 10, 10, 0, -5);
    addRoadtoCanvas(10, 10, 10, 0,  8);
    addRoadtoCanvas(10, 10, 10, 0,  5);
    addRoadtoCanvas(10, 10, 10, 0, -7);
    addRoadtoCanvas(10, 10, 10, 0,  5);
    addRoadtoCanvas(10, 10, 10, 0, -2);
}

function addDownhillToEnd(num) {
    num = num || 200;
    addRoadtoCanvas(num, num, num, -ROAD_OBJ.CURVE_PART.EASY, -lastY()/segmentLength);
}

function resetallRoads() {
    roadSegmentsArray = [];

    addStraightPortion(ROAD_OBJ.LEN.SHORT);
    addLowRollingHills();
    addStraightCurves();
    addCurvePortion(ROAD_OBJ.LEN.MED, ROAD_OBJ.CURVE_PART.MED, ROAD_OBJ.HILL_PART.LOW);
    addBumpyPortions();
    addLowRollingHills();
    addCurvePortion(ROAD_OBJ.LEN.LONG*2, ROAD_OBJ.CURVE_PART.MED, ROAD_OBJ.HILL_PART.MED);
    addStraightPortion();
    addHillPortion(ROAD_OBJ.LEN.MED, ROAD_OBJ.HILL_PART.HI);
    addStraightCurves();
    addCurvePortion(ROAD_OBJ.LEN.LONG, -ROAD_OBJ.CURVE_PART.MED, ROAD_OBJ.HILL_PART.NONE);
    addHillPortion(ROAD_OBJ.LEN.LONG, ROAD_OBJ.HILL_PART.HI);
    addCurvePortion(ROAD_OBJ.LEN.LONG, ROAD_OBJ.CURVE_PART.MED, -ROAD_OBJ.HILL_PART.LOW);
    addBumpyPortions();
    addHillPortion(ROAD_OBJ.LEN.LONG, -ROAD_OBJ.HILL_PART.MED);
    addStraightPortion();
    addStraightCurves();
    addDownhillToEnd();

    resetGameObjects();
    resetallCars();

    roadSegmentsArray[getSegment(playerZ).index + 2].color = COLORS.START;
    roadSegmentsArray[getSegment(playerZ).index + 3].color = COLORS.START;
    for(let n = 0 ; n < rumbleLength ; n++)
        roadSegmentsArray[roadSegmentsArray.length-1-n].color = COLORS.FINISH;

    trackLength = roadSegmentsArray.length * segmentLength;
}

function resetGameObjects() {
    let n, i;

    addGameObject(20,  GAMEOBJECTS.BANNERNO7, 1);
    addGameObject(40,  GAMEOBJECTS.BANNERNO6, 1);
    addGameObject(60,  GAMEOBJECTS.BANNERNO8, 1);
    addGameObject(80,  GAMEOBJECTS.BANNERNO9, 1);
    addGameObject(100, GAMEOBJECTS.BANNERNO1, 1);
    addGameObject(120, GAMEOBJECTS.BANNERNO2, 1);
    addGameObject(140, GAMEOBJECTS.BANNERNO3, 1);
    addGameObject(160, GAMEOBJECTS.BANNERNO4, 1);
    addGameObject(180, GAMEOBJECTS.BANNERNO5, 1);

    addGameObject(240, GAMEOBJECTS.BANNERNO7, 1.2);
    addGameObject(240, GAMEOBJECTS.BANNERNO6,  1.2);
    addGameObject(roadSegmentsArray.length - 25, GAMEOBJECTS.BANNERNO7, 1.2);
    addGameObject(roadSegmentsArray.length - 25, GAMEOBJECTS.BANNERNO6,  1.2);

    for(n = 10 ; n < 200 ; n += 4 + Math.floor(n/100)) {
        addGameObject(n, GAMEOBJECTS.TREENO0, -0.5 - Math.random()*0.5);
        addGameObject(n, GAMEOBJECTS.TREENO0,   -1 - Math.random()*2);
    }

    for(n = 250 ; n < 1000 ; n += 5) {
        addGameObject(n,     GAMEOBJECTS.COLUMN, 1);
        addGameObject(n + Math.round(0 + (5 - 0) * Math.random()), GAMEOBJECTS.TREENO1, -1 - (Math.random() * 2));
        addGameObject(n + Math.round(0 + (5 - 0) * Math.random()), GAMEOBJECTS.TREENO2, -1 - (Math.random() * 2));
    }

    for(n = 200 ; n < roadSegmentsArray.length ; n += 3) {
        addGameObject(n, getRandomArrElem(GAMEOBJECTS.TREES), -1 * (2 + Math.random() * 5));
    }

    let side, plant, plantOffset, building, dinasour1, dinasour2, buildingOffset, dinasourOffset;
    for(n = 1000 ; n < (roadSegmentsArray.length-50) ; n += 60) {
        side      = 1;
        dinasour1 = getRandomArrElem(GAMEOBJECTS.DINASOURS);
        dinasour2 = getRandomArrElem(GAMEOBJECTS.DINASOURS);
        dinasourOffset = side + Math.random() * 5;
        addGameObject(n , getRandomArrElem(GAMEOBJECTS.BANNERS), side + .5);
        addGameObject(n - 5 , dinasour1, dinasourOffset);
        addGameObject(n + 5 , dinasour2, dinasourOffset);
        for(i = 0 ; i < 10 ; i++) {
            plant = getRandomArrElem(GAMEOBJECTS.TREES);
            building = getRandomArrElem(GAMEOBJECTS.BUILDINGS);
            
            plantOffset = -1 * (1.5 + Math.random());
            buildingOffset = 2 + (10 * Math.random());
            
            addGameObject(n + Math.round(50 * Math.random()), plant, plantOffset);
            addGameObject(n + 10 + Math.round(30 * Math.random()), building, buildingOffset);			
        }
            
    }

}

function resetallCars() {
    carsArray = [];
    let car, segment, offset, z, gameObject, speed;
    for (let n = 0 ; n < totalCars ; n++) {
        offset = Math.random() * getRandomArrElem([-0.8, 0.8]);
        z      = Math.floor(Math.random() * roadSegmentsArray.length) * segmentLength;
        gameObject = getRandomArrElem(GAMEOBJECTS.CARS);
        speed  = maxSpeed/4 + Math.random() * maxSpeed/(gameObject == GAMEOBJECTS.POLICE_VAN ? 4 : 2);
        car = { offset: offset, z: z, gameObject: gameObject, speed: speed };
        segment = getSegment(car.z);
        segment.carsArray.push(car);
        carsArray.push(car);
    }
}

function resetCanvas() {
    canvas.width  = canvasWidth;
    canvas.height = canvasHeight;
    if(roadSegmentsArray.length==0)
        resetallRoads(); 
}

run({ images: ["background", "sprites"],
    keys: [
        { key: KEY.LEFT, mode: 'down', action: function() { keyLeft   = true;  } },
        { key: KEY.RIGHT, mode: 'down', action: function() { keyRight  = true;  } },
        { key: KEY.UP, mode: 'down', action: function() { keyFaster = true;  } },
        { key: KEY.DOWN, mode: 'down', action: function() { keySlower = true;  } },
        { key: KEY.LEFT, mode: 'up',   action: function() { keyLeft   = false; } },
        { key: KEY.RIGHT, mode: 'up',   action: function() { keyRight  = false; } },
        { key: KEY.UP, mode: 'up',   action: function() { keyFaster = false; } },
        { key: KEY.DOWN, mode: 'up',   action: function() { keySlower = false; } }
    ],
    ready: function(images) {
        backgroundImages = images[0];
        gameObjects    = images[1];
        resetCanvas();
    }
});