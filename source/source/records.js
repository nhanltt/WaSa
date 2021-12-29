// Initialize Firebase
var firebaseRef = {};
// เพื่อสร้างตัวแปร Global ไว้ส่งค่าไปใช้ในส่วนอื่น ๆ
var backup_datas = {};

angular.module('firebaseApp', []).service('firebaseService', function() {
    // สร้าง Function เพื่อ init Firebase เข้ากับ app เรา
    this.initFirebase = function() {
        var config = {
            apiKey: "AIzaSyDDp-TZedDdUaBg4SmiJcN0AQs5LUM1g7k",
            authDomain: "helloworld-f400c.firebaseapp.com",
            databaseURL: "https://helloworld-f400c-default-rtdb.asia-southeast1.firebasedatabase.app/"
        };
        firebase.initializeApp(config);
        firebaseRef = firebase.database().ref("Set"); 
        firebaseRef_sundir = firebase.database().ref("Push/sun_direction/brightness").limitToLast(1800);
        firebaseRef_wind_sp = firebase.database().ref("Push/wind_Speed").limitToLast(1800); 
    }
    // สร้าง Function เพื่อเก็บ event ของ Firebase
    this.eventFirebase = function() {
        firebaseRef.on('value', function(data) {
            const datas = Object.values(data.val());

            // ใส่ค่าเข้าตัวแปร Global
            backup_datas = datas;

            console.log(backup_datas);

            // console.log(datas);

            if (datas[6] <= 0) {
                datas[6] = 0;
                backup_datas[6] = 0;
            }

            var brt_percent = Number((((1024 - datas[10]['brightness'])/1024)*100).toFixed(2));
            var max_brt = Number((((1024 - datas[9]['brightness'])/1024)*100).toFixed(2));
            var min_brt = Number((((1024 - datas[8]['brightness'])/1024)*100).toFixed(2));
            var id = datas[10]['light_id'];
            
            document.getElementById('speed').innerHTML ="Current wind speed: " + datas[11];
            g1.refresh(backup_datas[11]);
            document.getElementById('max_speed').innerHTML ="Maximum wind speed: " + datas[8]['wind_Speed'];
            document.getElementById('min_speed').innerHTML ="Minimum wind speed: " + datas[9]['wind_Speed'];
            document.getElementById('avg_speed').innerHTML ="Average wind speed: ";

            document.getElementById('inten').innerHTML ="Current sun intensity: " + brt_percent;
            g2.refresh(brt_percent);
            document.getElementById('max_inten').innerHTML ="Maximum sun intensity: " + max_brt;
            document.getElementById('min_inten').innerHTML ="Minimum sun intensity: " + min_brt;

            var sun_angle = (id - 1) * 45;
            document.getElementById('sun_angle').innerHTML ="Current sun intensity: " + sun_angle + " deg \n from East";
            g3.refresh(sun_angle);

            var w_dir = ['North', 'Northeast by north', 'Northeast', 'Northeast by east', 'East', 'Southeast by east', 'Southeast', 'Southeast by south', 'South', 'Southwest by south', 'Southwest', 'Southwest by west', 'West', 'Northwest by west', 'Northwest', 'Northwest by north']
            var direction = document.querySelector('div.compass');
            var wind_direction = (22.5 * datas[7]).toString();
            direction.style.setProperty('--element-angle', wind_direction);
            document.getElementById('wind_dir').innerHTML = "Wind from: " + w_dir[datas[7]];
            console.log('good');

        })

        firebaseRef_sundir.on('value', function(data) {
            const datas_sundir = Object.values(data.val());
            var sum = 0;
            var avg_suninten = 0;
            var n = datas_sundir.length;

            for (i = 0; i < n; i++) {
                sum += datas_sundir[i];
            }
            avg_suninten = (sum/n);
            avg_suninten = Number((((1024 - avg_suninten)/1024)*100).toFixed(2))
            document.getElementById('avg_inten').innerHTML ="Average sun intensity: " + avg_suninten;
        })
        
        firebaseRef_wind_sp.on('value', function(data) {
            const datas_wsp = Object.values(data.val());
            var sum = 0;
            var avg_wsp = 0;
            var n = datas_wsp.length;

            for (i = 0; i < n; i++) {
                sum += datas_wsp[i];
            }
            avg_wsp = (sum/n);
            avg_wsp = Number((((1024 - avg_wsp)/1024)*100).toFixed(2))
            document.getElementById('avg_speed').innerHTML ="Average wind speed: " + avg_wsp;
        })
    }
    this.push = function(title, content) {}
    }).controller('firebaseCtrl', function(firebaseService) {
        // เรียกใช้งาน firebaseService ที่เราสร้างไว้
        firebaseService.initFirebase();
        firebaseService.eventFirebase();
});

