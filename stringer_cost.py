# -*- coding: utf-8 -*-
#大客壁板算例
import math
from openpyxl import load_workbook


# char = [chr(code) for code in range(945,970)]
# codelist = [code for code in range(945,970)]
# print(char)





def excel_index_match(lookup_value, lookup_range, result_range):
    """优化后的INDEX-MATCH函数"""
    # 添加空值检查
    if lookup_value is None:
        return None

    for lookup_cell, result_cell in zip(lookup_range, result_range):
        if lookup_cell.value == lookup_value:
            return result_cell.value
    return None


def t_CH_yunsuputie(step,G,η,ν):
    #功能：工型长桁铺贴成本模型，基于重量不考虑铺层角度的匀速铺贴模型.
    #t：工时，min.
    #step:准备时间，min.
    #G:净重.
    #η:材料利用率.
    #ν:铺贴速率，kg/min.
    t_zhongliang = step + G / (η * ν)
    return t_zhongliang
# a = t_CH_yunsuputie(30,14.76190476,0.21,0.010042112)
# print(a)
def tman_CH_guangyan(setup,delay,χ,ν,τ=0,Noper=1):
    #功能：工型长桁广延型工序成本模型，计算工装准备、下料、定位、制袋、脱模、机加、检测等.
    #tman：工时，min.
    #setup:准备时间，min.
    #delay:延迟时间,min.
    #χ:特征尺寸，kg、mm、m2.
    #ν:速率，kg/min、mm/min、m2/min.
    #τ:根据目前研制进展，τ取0.
    #Noper:不知道是啥，暂记为1.

    tman_guanyan = setup + (delay + math.sqrt(math.pow(χ/ν,2.0)+ 2.0 * τ* χ/ν)) * Noper
    return tman_guanyan
# a = tman_CH_guangyan(30,0,14.76190476,0.091461616)
# print(a)

def tman_CH_guhua(setup,delay,χ,ν,tcure=190,Noper=1):
    #功能：工型长桁固化成本模型，为传热模型，固化时间与材料、结构复杂度、工装等均有关系.
    #tman：工时，min.
    #setup:准备时间，min.
    #delay:延迟时间,min.
    #χ:特征尺寸，kg、mm、m2.注：χ=3.33m.
    #ν0:速率，kg/min、mm/min、m2/min.注：工型长桁长度＜5m，为0.0082;≥5m，为0.0077.
    #tcure:保温时间，针对工型长桁，固化程序中设定了保温平台，保温时间为两个时间，分别为60min和130min，总计190min.
    #Noper:不知道是啥，暂记为1.

    tman_guhua = setup + (delay + χ/ν + tcure) * Noper
    return tman_guhua
# b = tman_CH_guhua(30,0,3330,0.0077)
# print(b)
#Labor Cost = (Man-Hours × Labor Rate) + (Machine Hours × Machine Rate)
def Part_Labor_Cost_CH(Man_Hours,Labor_Rate,Machine_Hours,Machine_Rate):
    #功能：计算工型长桁各工序下的工时费.
    #Man_Hours：人工工时，min.
    #Labor_Rate:人工费率，元/h.
    #Machine_Hours:设备加工时间,min.
    #Machine_Rate:设备费率，元/h.
    Part_Labor_Cost = Man_Hours * Labor_Rate / 60 + Machine_Hours * Machine_Rate / 60
    return Part_Labor_Cost
#Cost per unit = Material cost per unit + Labor cost per unit
def Single_Part_Cost_CH(Material_cost_per_unit,Labor_cost_per_unit):
    #功能：计算工型长桁单件成本.
    #Material_cost_per_unit：单件材料费，元.
    #Labor_cost_per_unit:单件工时费，元.
    Single_Part_Cost = Material_cost_per_unit + Labor_cost_per_unit
    return Single_Part_Cost




