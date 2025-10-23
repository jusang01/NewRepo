# from http.client import HTTPException
from flask_cors import CORS
from flask import jsonify,request
import json
from flask import Blueprint, Flask, jsonify,make_response
from flask_docs import ApiDoc
import pandas as pd
import json
import os

from gevent import pywsgi

app = Flask(__name__)
cors_Msg = CORS(app, resources={r"/getMsg": {"origins": "*"}})


app.config["API_DOC_MEMBER"] = ["api"]
app.config["API_DOC_URL_PREFIX"] = "/docs"

def json_response(code,error,status_code):
    response=make_response(json.dumps(dict(code=code,error=error)),status_code)
    response.headers['Content-Type']='application/json;charset=utf-8'
    return response

ApiDoc(
    app,
    title="Portfolio",
    version="1.0.0",
    description="A Portfolio API",
)
api = Blueprint("api", __name__)

class User:
    def __init__(self, username, password, role="user"):
        self.username = username
        self.password = password
        self.role = role

users = [
            User("admin", "admin123", "admin"),
            User("user1", "password1"),
            User("user2", "password2")
        ]

custom_columns = ["单件材料费", "工时(h)", "工时费", "单件成本(元)"]

current_dir = os.path.dirname(os.path.abspath(__file__))
file_path = current_dir+"\成本模型.xlsx"

@app.route('/getMsg')
def hello_world():
    return 'Hello World!'

falg = False


@app.route('/login', methods=['POST'])
def login():
    data = json.loads(request.data)
    userName = data['userName']
    password = data['password']
    res_data = {"message": "登录失败", "role": "用户名或密码错误"}
    for user in users:
        if user.username == userName and user.password == password:
            print(f"登录成功! 欢迎, {user.username}")
            falg = True
            if user.role == "admin":
                res_data = {"message": "登录成功", "role": "管理员权限"}
            else:
                res_data = {"message": "登录成功", "role": "普通权限"}

    print("用户名或密码错误")
    return jsonify(res_data)

@app.route('/logout', methods=['POST'])
def logout():
    falg = False
    res_data = {"message": "退出成功"}
    print(falg)
    return jsonify(res_data)

@app.route('/readPartDatas',methods=['POST'])
def readPartDatas():
    data = json.loads(request.data)
    type = data['type']
    number = data['number']
    if type == 1:
        return get_part_device_data(number)
    if type == 2:
        return get_optimized_last_row(number)



app.register_blueprint(api, url_prefix="/")

def get_part_device_data(number):
    result = {}
    df = pd.read_excel(file_path, sheet_name=number)
    df = df.iloc[:-5] if len(df) > 2 else df
    part_device_data = df.iloc[:, 0:2].fillna(0)
    temp = number+1
    tempStr = 'Part'+str(temp)
    part_device_data.columns = [tempStr, '设备名称']
    data_dict = part_device_data.to_dict('records')
    result['data'] = data_dict

    return json.dumps(result, ensure_ascii=False, indent=2)


def get_optimized_last_row(number):
    result = {}
    df = pd.read_excel(file_path, sheet_name=number)
    if len(df) > 0:
        last_row = df.iloc[-1].values
        row_data = {custom_columns[i]: val for i, val in enumerate(last_row)}
        result['data'] = row_data

    return json.dumps(result, ensure_ascii=False, indent=2)


if __name__ == '__main__':

    server = pywsgi.WSGIServer(('0.0.0.0', 5000), app)
    server.serve_forever()
