# 仕様書
## コード
* test.json

    	{
            "id"  :"test",
            "obj" :{
                "key1":"data1",
                "key2":"data2"
            }
            "array":[1,2,3,4]
        }
* Ideal

        obj["id"] //test
        obj["obj"]["key1"] //data1
        obj["obj"]["key2"] //data2

* 途中で解釈を変える

        id = test,
        obj::key1 = data1,
        obj::data2= data2

