# 名称：服务端处理程序
# 时间：20200128
# 作者：voyager
# 功能：计算地点之间的距离
# 注意：


def dist_count(place_loc, place_num):
    place_dist = np.zeros((place_num, place_num))
    for i in range(place_num):
        for j in range(place_num):
            place_dist[i, j] = np.sqrt(np.square(place_loc[i, 0] - place_loc[j, 0])
                                       + np.square(place_loc[i, 1] - place_loc[j, 1])) / 180 * np.pi * 6370
    return place_dist



place_loc = np.loadtxt('data.txt')
place_dist = dist_count(place_loc, 25)
np.savetxt("result.txt", place_dist)
