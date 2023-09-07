sudo -u postgres psql -p 5433 -d bb -a -f sql/sys.sql

# sudo -u postgres: 使用 sudo 命令允许你以其他用户的身份执行命令，这里指的是以 postgres 用户身份。在默认情况下，postgres 是 PostgreSQL 的默认超级用户。
# psql: 是 PostgreSQL 的命令行客户端。
# -p 5433: 指定要连接的 PostgreSQL 服务器的端口号。在这里，端口号是 5433。通常，PostgreSQL 的默认端口号是 5432，但在这个命令中，它被设置为 5433。
# -d bb: 使用 -d 选项指定要连接的数据库名称，这里的数据库名称是 bb。
# -a: 表示在发送到服务器之前先回显所有查询。
# -f sql/sys.sql: 使用 -f 选项指定从文件执行 SQL 命令。这里，它从 sql/sys.sql 文件执行 SQL 命令。

sudo -u postgres psql -p 5433

sudo -u postgres psql -p 5433 -d bb -a -f sql/recent_proc.sql