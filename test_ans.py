import requests
import json
#ans = requests.get("http://162.243.126.219:5000/api/tasks/Id-AuId").text
ans = requests.get("http://162.243.126.219:5000/api/tasks/AuId-Id").text
#ans = requests.get("http://162.243.126.219:5000/api/tasks/AuId-AuId").text
#ans = requests.get("http://162.243.126.219:5000/api/tasks/Id-Id").text
ans = json.loads(ans)
task = ans["task"]
print 'task:', task


data = {}
data["task"] = task
data['time'] = 0
data['ans'] = requests.get("http://localhost/query?id1=" + str(task[0]) + "&id2=" + str(task[1])).text
print data['ans']
data['ans'] = json.loads(data['ans'])

print 'service return:', requests.post("http://162.243.126.219:5000/api/score", data=json.dumps(data)).text





