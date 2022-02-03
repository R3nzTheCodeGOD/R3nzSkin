import configparser
import json
import os
# from json_merger import Merger
# from json_merger.config import UnifierOps, DictMergerOps

lolskin_config_path = "C:\Fraps\data\My\Config.ini"


def skin_to_dictionary(path):
    config = configparser.ConfigParser()

    config.read(path, encoding="utf-8")
    ini_dict = {}
    for k, v in config.items():
        ini_dict[k] = dict(config.items(k))
    # print(ini_dict)
    hero_skin = ini_dict["SKIN_CHAMPION_ACTIVED"]
    del hero_skin['custom_file']

    return hero_skin


def forward_data(hero_skin: dict):
    # 处理为skin++数据
    dict_from_list = {}

    for key, value in hero_skin.items():
        key = key.capitalize()
        key = key + ".current_combo_skin_index"
        value = int(value)
        # + int(1)
        # print(key,value)
        dict_from_list[key] = value

    return dict_from_list


# def open_skin_json():
#     skin_json = {}
#     with open('py\R3nzSkin.json', 'r') as fp:
#         skin_json = json.load(fp)

#     # print(skin_json)
#     return skin_json


if os.path.exists(lolskin_config_path):
    hero_skin = skin_to_dictionary(lolskin_config_path)
else:
    lolskin_config_path = input("lolskin配置文件 举例:\t" + lolskin_config_path + "\n")
to_skin = forward_data(hero_skin=hero_skin)
# print(to_skin)

json_str = json.dumps(to_skin)
print("JSON 对象：", json_str)
with open('data.json', 'w') as fp:
    json.dump(to_skin, fp)

# TODO合并json文件
