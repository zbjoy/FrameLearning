#	docker

+ docker push

  ```dockerfile
  docker commit `container_name` `image_hub`:`tag_name`
  docker tag `本地镜像仓库`:`本地镜像标签` `远程仓库名`:`远程镜像标签`
  docker login
  docker push `远程仓库名`:`远程仓库镜像标签`
  ```

+ docker run

  ```dockerfile
  docker run `docker_image_name`
  
  参数:
  	-ti 终端(terminal)运行, 终端结束则容器停止运行
  	--rm 容器(container)运行结束就删除
  	-d 守护进程(daemon)的显示运行
  	-v `宿主机文件路径`:`容器路径`
  	-p `宿主机端口`:`容器暴露的端口`
  	-P `宿主机自己随机选择合适端口`    # 一般该命令与 Dockerfile 的 EXPOSE `container_port` 一起使用
  ```

+ docker pull

  ```dockerfile
  docker pull `远程仓库名`:`远程镜像标签`
  # eg: docker pull ubuntu
  ```

  

+ docker start

  ```dockerfile
  docker -ai start `container_id`
  ```

+ docker stop

  ```dockerfile
  docker stop `container_id`
  ```

+ docker ps -a

+ docker rm

  ```dockerfile
  docker rm `container_id`
  ```

+ docker rmi

  ```dockerfile
  docker rmi `image_id`
  ```

+ docker cp

  ```dockerfile
  docker cp `宿主机文件路径` `container_id`:`容器文件路径`
  ```

+ docker expost

  ```dockerfile
  docker expost -o `名字`.tar `container_id`
  ```

+ docker import

  ```dockerfile
  docker import `名字`.tar `起一个容器名`:`起一个容器标签`
  ```

+ docker build

  ```dockerfile
  docker build -t `container_name`:`tag名字` `Dockerfile的路径`
  ```

  