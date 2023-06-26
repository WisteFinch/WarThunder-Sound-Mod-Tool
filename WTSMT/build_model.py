import json
import tkinter
import os
from tkinter.filedialog import askdirectory, asksaveasfile
from pathlib import Path

def selectDir():
    path = askdirectory(title='指定模型文件夹')
    return path

def saveFilePath():
    path = asksaveasfile(title='指定保存位置', filetypes=[('JSON Documnet','*.json')], defaultextension='.json')
    return path

def typeToPath(type:str):
    s = type.replace('.', '/')
    s = s.replace('default', '')
    s = s.replace('.default', '')
    return s

# 获取语种列表
def getLangs(path:str, data:dict):
    lang_list = []
    p = Path(path)
    lang_path_list = [i for i in p.iterdir() if i.is_dir()]
    for i in lang_path_list:
        lang_list.append({'id':i.name, 'name': i.name})
    data['langs'] = lang_list

# 获取种类列表
def getTypes(path:str, prefix:str, data:dict):
    type_list = []
    p = Path(path)
    flag = False
    for i in p.iterdir():
        if i.is_dir():
            if i.name != 'CVS':
                child = getTypes(p / i.name, i.name, data)
                if child == []:
                    type_list.append(prefix + '.' + i.name)
                else:
                    for j in child:
                        type_list.append(prefix + '.' + j)
        else:
            flag = True
    if flag and type_list != []:
        type_list.append(prefix + '.default')
    return type_list

# 分析文件
def analyseFiles(path:str, data:dict):
    p = Path(path)
    langs = data['langs']
    types = data['types']
    items_dict = {}
    for lang in langs:
        lang_path = Path(p / lang['id'])
        for type in types:
            dir = Path(lang_path / typeToPath(type))
            if not dir.exists():
                continue
            for file in dir.iterdir():
                if file.is_file():
                    if not file.stem in items_dict.keys():
                        items_dict[file.stem] = {}
                        items_dict[file.stem]['file_name'] = file.name
                        items_dict[file.stem]['id'] = file.stem
                        items_dict[file.stem]['mapping'] = {}
                    mapping_dict = items_dict[file.stem]['mapping']
                    if not type in mapping_dict.keys():
                        mapping_dict[type] = {}    
                        mapping_dict[type]['type'] = type
                        mapping_dict[type]['langs'] = {}
                    langs_dict = mapping_dict[type]['langs']
                    if not lang['id'] in langs_dict.keys():
                        langs_dict[lang['id']] = {}
                        langs_dict[lang['id']]['lang'] = lang['id']
                        langs_dict[lang['id']]['list'] = []
                    file_list = langs_dict[lang['id']]['list']
                    file_list.append(typeToPath(type) + '/' + file.name)
    
    items_list = []
    for file in items_dict.values():
        file_dict = {'file_name':file['file_name'], 'id':file['id']}
        mapping_list = []
        for mapping in file['mapping'].values():
            mapping_item = {'type':mapping['type']}
            langs_list = []
            for lang in mapping['langs'].values():
                lang_item = {'lang':lang['lang'], 'list':lang['list']}
                langs_list.append(lang_item)
            mapping_item['langs'] = langs_list
            mapping_list.append(mapping_item)
        file_dict['mapping'] = mapping_list
        items_list.append(file_dict)
    data['items'] = items_list

def __main__():
    # 获取文件夹位置
    ori_path = Path(selectDir())
    if not ori_path.exists:
        print('Invalid path')

    # 创建字典
    data = {}

    # 获取名称
    name = input('模型名称:')
    data['name'] = name

    # 获取语种列表
    getLangs(str(ori_path), data)
    
    # 获取种类列表
    types_list = getTypes(ori_path / data['langs'][0]['id'], '', data)
    for i in range(len(types_list)):
        if types_list[i][0] == '.':
            types_list[i] = types_list[i][1:]
    data['types'] = types_list

    # 分析文件
    analyseFiles(ori_path, data)

    #保存文件
    save_path = saveFilePath()
    json.dump(data, save_path)
    

if __name__ == '__main__':
    __main__()