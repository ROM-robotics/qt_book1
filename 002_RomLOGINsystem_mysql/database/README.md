To set up a MySQL container with **MySQL Admin** (or a web-based MySQL client like **phpMyAdmin**) running in a container, you can follow these steps using **Docker Compose** to orchestrate both the MySQL server and the phpMyAdmin client in separate containers.

### Step-by-Step Guide: MySQL + phpMyAdmin in Docker Containers

#### Prerequisites:
- **Docker** and **Docker Compose** should be installed on your system. If you don't have them installed, follow the instructions here:
  - [Install Docker](https://docs.docker.com/get-docker/)
  - [Install Docker Compose](https://docs.docker.com/compose/install/)

### 1. **Create a `docker-compose.yml` File**

In your project directory, create a file named `docker-compose.yml`. This file will define the configuration for both MySQL and phpMyAdmin services.

Here’s an example `docker-compose.yml` file:

```yaml
version: '3.1'

services:
  mysql:
    image: mysql:5.7
    container_name: mysql-container
    environment:
      MYSQL_ROOT_PASSWORD: my-secret-pw   # Set your root password
      MYSQL_DATABASE: mydatabase          # Optional: Default database to create
    ports:
      - "3306:3306"  # Expose MySQL port
    volumes:
      - mysql-data:/var/lib/mysql  # Persist MySQL data on your local machine
    networks:
      - mysql-network

  phpmyadmin:
    image: phpmyadmin/phpmyadmin
    container_name: phpmyadmin-container
    environment:
      PMA_HOST: mysql        # The MySQL service name (as defined in the docker-compose.yml)
      PMA_PORT: 3306         # Port MySQL is running on (default: 3306)
      MYSQL_ROOT_PASSWORD: my-secret-pw  # The same root password used for MySQL
    ports:
      - "8080:80"  # Expose phpMyAdmin on port 8080
    networks:
      - mysql-network
    depends_on:
      - mysql

networks:
  mysql-network:
    driver: bridge

volumes:
  mysql-data:
    driver: local
```

### Explanation:
- **MySQL Container**:
  - Uses the official `mysql:5.7` image.
  - The `MYSQL_ROOT_PASSWORD` environment variable sets the root password for MySQL (replace `my-secret-pw` with a secure password of your choice).
  - The `MYSQL_DATABASE` variable is optional and creates a default database on initialization.
  - The `volumes` section ensures MySQL data persists even when the container is stopped or removed.
  - Exposes MySQL on port `3306`.

- **phpMyAdmin Container**:
  - Uses the official `phpmyadmin/phpmyadmin` image.
  - The `PMA_HOST` and `PMA_PORT` environment variables point phpMyAdmin to the MySQL container.
  - Exposes phpMyAdmin on port `8080`, so you can access it at `http://localhost:8080` on your host machine.
  - The `depends_on` directive ensures that the MySQL container starts before phpMyAdmin.

- **Networks**:
  - Both containers are connected to the same Docker network (`mysql-network`), allowing phpMyAdmin to communicate with MySQL by using the service name `mysql` (as specified in the `PMA_HOST` variable).

- **Volumes**:
  - A Docker volume (`mysql-data`) is used to persist MySQL data outside of the container.

### 2. **Start the Containers with Docker Compose**

Now that you have the `docker-compose.yml` file set up, you can start both containers.

1. Open a terminal in the directory where your `docker-compose.yml` file is located.

2. Run the following command to start the containers:

   ```bash
   docker-compose up -d
   ```

   This will download the necessary images and start the MySQL and phpMyAdmin containers in detached mode (`-d`).

### 3. **Access phpMyAdmin**

After the containers are up and running, you can access **phpMyAdmin** via your web browser:

- Open your browser and go to `http://localhost:8080`.
- Log in using the following credentials:
  - **Username**: `root`
  - **Password**: `my-secret-pw` (or whatever you set in the `MYSQL_ROOT_PASSWORD` environment variable)

Now you should be able to interact with your MySQL database through the phpMyAdmin interface.

### 4. **Stopping and Managing Containers**

- **Stop the containers**:

  To stop the running containers, use:

  ```bash
  docker-compose down
  ```

- **Restart the containers**:

  To restart the containers (after making changes), use:

  ```bash
  docker-compose restart
  ```

- **Remove the containers**:

  If you want to completely remove the containers (but retain the data), use:

  ```bash
  docker-compose down --volumes
  ```

### 5. **Customizing the Setup**
You can further customize your setup by:
- Adding additional services or configuration options.
- Exposing more ports or services as needed.
- Mounting your local directory to `/var/www/html` in the MySQL container if you want to serve PHP files directly via Apache.

For example, if you want to add a **custom PHP website**, you can adjust the `phpMyAdmin` setup to use a specific directory for web files by adding an additional volume:

```yaml
  phpmyadmin:
    image: phpmyadmin/phpmyadmin
    container_name: phpmyadmin-container
    environment:
      PMA_HOST: mysql
      PMA_PORT: 3306
      MYSQL_ROOT_PASSWORD: my-secret-pw
    ports:
      - "8080:80"
    networks:
      - mysql-network
    volumes:
      - ./my-website:/var/www/html  # Mount a local website folder
    depends_on:
      - mysql
```

### Recap:
1. **docker-compose.yml**: Defines services for both **MySQL** and **phpMyAdmin**.
2. **Start the containers**: `docker-compose up -d`.
3. **Access phpMyAdmin**: Navigate to `http://localhost:8080` in your browser.
4. **Stop and remove containers**: `docker-compose down`.

This setup provides a clean, isolated environment for MySQL and phpMyAdmin, and using Docker Compose simplifies managing multiple containers and their configurations.