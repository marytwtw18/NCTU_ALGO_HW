input為無數個小方塊(左下及右上角座標)，透過stl內建的map
儲存(2d interval tree)，題目要求五個mode:

1.insert
2.delete
3.search特定方塊編號
4.回傳特定左下角座標之最小面積編號
5.design rule check，給定一個方塊，確認所有包
圍住該方塊的方塊有沒有violate(邊界distance<20)

implemented by stl map(rb tree)
