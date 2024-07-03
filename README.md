# 从有坏道硬盘中捞文件的工具
## 有什么用

我有一个大硬盘用于备份，里面放了大量RAR打包的文件，每一个文件都加了15%的恢复记录，

时间久远，硬盘有些坏块

教科书教我们的标准做法是用ddrescue类似的工具，先把整个硬盘copy为一个镜像，

然后从镜像里捞文件。但是，我的硬盘有14T，我没有任何额外地方存放14T的硬盘镜像T_T

如果直接从硬盘上copy，通常的工具，碰到坏道就会报错退出，从而即使有一点点错误，也要丢弃整个文件

这个工具这时候就排上用场了，我用这个工具直接从硬盘上把文件忽略坏块读出来，然后

利用rar的恢复记录恢复文件

## 原理
没啥，就是先用大缓冲区读文件，出错了逐个扇区读，错的扇区补0，这样既能保证速度又能较好处理坏扇区，就这一点点技巧。。

## 还有什么可能的改进
读取超时应该是可设置的，用异步IO可以实现，当前也能用，不想折腾了吧

# A tool used to retrieve files from hard drives with bad sectors
I have a large hard drive for backup, which contains a large number of RAR-packaged files, and a 15% recovery record is added to each file.

It’s been a long time, and the hard drive has some bad sectors.

The standard approach taught in textbooks is to use a tool like ddrescue to first copy the entire hard disk into an image.

Then grab the files from the image. However, my hard drive has 14T and I don’t have any extra place to store the 14T hard drive image T_T

If you copy directly from the hard disk, ordinary tools will report an error and exit when encountering bad sectors, 

so even if there is a little error, the entire file will be discarded.

This tool comes in handy at this time. I use this tool to read files directly from the hard disk ignoring bad blocks, and then

Recover files using rar recovery records
