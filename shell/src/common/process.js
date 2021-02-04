var cprocess = require('child_process');

module.exports = (function () {
    return {
        exec: function (fileName, filePath, title) {
            cprocess.execFile(
                fileName,
                null,
                {
                    cwd: filePath,
                },
                function (error, stdout, stderr) {
                    if (error) {
                        console.log(
                            (title || fileName) + '启动失败！错误消息：' + error
                        );
                    } else {
                        console.log((title || fileName) + '启动成功！');
                    }
                }
            );
        },
        kill: function (title) {
            let cmd = process.platform === 'win32' ? 'tasklist /v' : 'ps aux';
            cprocess.exec(cmd, function (err, stdout, stderr) {
                if (err) {
                    return console.error(err);
                }
                stdout.split('\n').filter((line) => {
                    if (line.indexOf(title) != -1) {
                        let processMessage = line.trim().split(/\s+/);
                        let processId = processMessage[1];
                        try {
                            process.kill(processId);
                            console.log(title + '已关闭！');
                        } catch (err) {
                            console.log(title + '已关闭！');
                        }
                    }
                });
            });
        },
    };
})();
