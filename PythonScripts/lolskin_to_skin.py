import configparser
import json
import os


def skin_to_dictionary(path):
    config = configparser.ConfigParser()

    config.read(path, encoding="utf-8")
    ini_dict = {}
    for key, values in config.items():
        ini_dict[key] = dict(config.items(key))
        print(values)
    lolskin_hero_skin = ini_dict["SKIN_CHAMPION_ACTIVED"]
    del lolskin_hero_skin['custom_file']

    return lolskin_hero_skin


def forward_data(lolskin_hero_skin: dict):
    # Convert lolskin's skin data to R3nzSkin
    dict_from_list = {}

    for key, value in lolskin_hero_skin.items():
        key = key.capitalize()
        key = key + ".current_combo_skin_index"
        value = int(value) + int(1)
        dict_from_list[key] = value

    return dict_from_list



if __name__ == '__main__':
    LOLSKIN_CONFIG_PATH = r"C:\Fraps\data\My\Config.ini"

    if os.path.exists(LOLSKIN_CONFIG_PATH):
        hero_skin = skin_to_dictionary(LOLSKIN_CONFIG_PATH)
    else:
        LOLSKIN_CONFIG_PATH = input(
            "lolskin Configuration file path Example:\t" + LOLSKIN_CONFIG_PATH + "\n")
    to_skin = forward_data(lolskin_hero_skin=hero_skin)

    json_str = json.dumps(to_skin)
    print("JSON 对象：", json_str)
    with open('data.json', 'w', encoding="utf-8") as fp:
        json.dump(to_skin, fp)