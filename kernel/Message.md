### 消息格式
- 打开文件夹

    ```json
    {
    	"id": "go_to_folder",
        "template_id": 100,
        "skin_id": 100
    }
    ```

    ```json
    {
        "id": "go_to_folder",
    	"result": 0,
    	"reason": ""
    }
    ```

- 压缩图片

    ```json
    {
    	"id": "compress",
    	"template_id": 100,
    	"skin_id": 200
    }
    ```

    ```json
    {
    	"id": "compress",
    	"result": -1,
    	"reason": "压缩失败"
    }
    ```

    

- 打包上传

    ```json
    {
    	"id": "upload",
    	"template_id": 100,
    	"skin_id": 200
    }
    ```

    ```json
    {
    	"id": "upload",
    	"result": 0,
        "reason": "",
    	"steps": ["合图", "序列化CSD", "打包", "上传"],
        "current": "合图"
    }
    ```



- 从服务器加载

    ```json
    {
    	"id": "download",
    	"template_id": 100,
    	"skin_id": 200
    }
    ```

    ```json
    {
    	"id": "download",
    	"result": 0,
    	"reason": "",
    	"steps": ["下载", "解析"],
    	"current": "下载"
    }
    ```

- 获取所有文件

    ```json
    {
    	"id": "files",
    	"template_id": 100,
    	"skin_id": 100
    }
    ```

    ```json
    {
    	"id": "files",
      "result": 0,
      "reason": '',
    	"files":[
    		{
    			"name": "1.png",
    			"type": "png",
    			"time": 12345566,
                "size": 1111,
    			"path": "/path"
    		}
    	]
    }
    ```

- 退出

    ```
    {
    	"id": "exit",
    }
    ```

    

- 打开||删除||重命名文件

    ```
    {
    	"id": "open_delete_rename_file",
    	"operation":"open",
    	"path":"",
    	"new_name":""
    }
    ```

    