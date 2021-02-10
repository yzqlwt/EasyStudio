import fs from 'fs';
import https from 'https';

function getOptions() {
    //生成请求头部
    var time = Date.now();
    let UserAgent = "Mozilla/5.0(WindowsNT10.0;Win64;x64)AppleWebKit/537.36(KHTML,likeGecko)Chrome/" + 59 + Math.round(Math.random() * 10) + ".0.3497." + Math.round(Math.random() * 100) + "Safari/537.36";
    var options = {
        method: "POST",
        hostname: "tinypng.com",
        path: "/web/shrink",
        headers: {
            rejectUnauthorized: false,
            "Postman-Token": (time -= 5000),
            "Cache-Control": "no-cache",
            "Content-Type": "application/x-www-form-urlencoded",
            "User-Agent": UserAgent,
            "X-Forwarded-For": getIp(),
            Cookie: "",
        },
        timeout:5000
    };
    return options;
}
function getIp() {
    var _ = {
        random: function (start, end) {
            return parseInt(Math.random() * (end - start) + start);
        },
    };
    var ip = _.random(1, 254) + "." + _.random(1, 254) + "." + _.random(1, 254) + "." + _.random(1, 254);
    return ip;
}

export const tinypng = function(path, callback) {
    const imageData = fs.readFileSync(path);
    if (!imageData) {
        return false;
    }
    let req = https.request(getOptions(), (res) => {
        res.on('data', (buf) => {
            let obj;
            try {
                obj = JSON.parse(buf.toString());
            } catch (error) {
                callback(new Error('解析返回值失败'));
            }
            if (obj.error) {
                callback(new Error(obj.error));
            } else {
                obj.path = path
                callback(obj);
            }
        });
    });
    console.log(req, "请求")
    req.write(imageData, 'binary');
    req.on('error', (err) => {
        callback(err);
    });
    req.end();
}
