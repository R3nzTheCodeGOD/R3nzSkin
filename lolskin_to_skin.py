import configparser
import json
import os
# from json_merger import Merger
# from json_merger.config import UnifierOps, DictMergerOps


def skin_to_dictionary(path):
    config = configparser.ConfigParser()

    config.read(path, encoding="utf-8")
    ini_dict = {}
    for key, values in config.items():
        ini_dict[key] = dict(config.items(key))
        print(values)
    # print(ini_dict)
    lolskin_hero_skin = ini_dict["SKIN_CHAMPION_ACTIVED"]
    del lolskin_hero_skin['custom_file']

    return lolskin_hero_skin


def forward_data(lolskin_hero_skin: dict):
    # 处理为skin++数据
    dict_from_list = {}

    for key, value in lolskin_hero_skin.items():
        key = key.capitalize()
        key = key + ".current_combo_skin_index"
        value = int(value) + int(1)
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

if __name__ == '__main__':
    LOLSKIN_CONFIG_PATH = r"C:\Fraps\data\My\Config.ini"

    if os.path.exists(LOLSKIN_CONFIG_PATH):
        hero_skin = skin_to_dictionary(LOLSKIN_CONFIG_PATH)
    else:
        LOLSKIN_CONFIG_PATH = input(
            "lolskin配置文件 举例:\t" + LOLSKIN_CONFIG_PATH + "\n")
    to_skin = forward_data(lolskin_hero_skin=hero_skin)
    # print(to_skin)

    json_str = json.dumps(to_skin)
    print("JSON 对象：", json_str)
    with open('data.json', 'w', encoding="utf-8") as fp:
        json.dump(to_skin, fp)

# TODO合并json文件
