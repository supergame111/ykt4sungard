<%@ page contentType="text/html; charset=GBK"%>
<%@ page import="com.kingstargroup.fdykt.KSConfiguration" %>
<%@ taglib uri="/WEB-INF/struts-html.tld" prefix="html"%>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
	<head>
		<title>ת�ʽ������</title>
		<%
		//String  cardno =  request.getSession().getAttribute("cardno")==null?"":(String) request.getSession().getAttribute("cardno");
		  String  password = request.getParameter("password")==null?"":request.getParameter("password").toString();
		  session.setAttribute("password",password);
		%>
		<script language="javascript">
		
        function addnum(num)
        {
		if(putinForm.nowinput.value=="1" && putinForm.money.value.length < 5 ){
			putinForm.money.value+=num;
		}
        }
        function cancelnum()
        {
		if(putinForm.nowinput.value=="1")
			putinForm.money.value=putinForm.money.value.substring(0,putinForm.money.value.length-1);		
        }      
    	 function check(){
   	 	
    	 	if(putinForm.money.value==''){
    	 		alert("������ת�ʽ�");
    	 		putinForm.money.focus();
    	 		return false;
    	 	}
    	 	
    	 	if(!fucCheckNUM(putinForm.money.value)){
    	 	 	alert("ת�ʽ��Ϊ���֣�");
    	 		putinForm.money.focus();
    	 		return false;
    	 	}
    	 	
    	 	if(putinForm.money.value<=0){
    	 		alert("ת�ʽ��Ӧ�ô���0��");
    	 		putinForm.money.focus();
    	 		return false;
    	 	}
    	 	
    	 	if(putinForm.money.value>1000){
    	 		alert("ת�ʽ�����");
    	 		putinForm.money.focus();
    	 		return false;
    	 	}
    	 
    	 	putinForm.submit();
        }
        
         function fucCheckNUM(NUM)
		{
		 var i,j,strTemp;
		 strTemp="0123456789";
		 if ( NUM.length== 0)
		  return true
		 for (i=0;i<NUM.length;i++)
		 {
		  j=strTemp.indexOf(NUM.charAt(i)); 
		  if (j==-1)
		  {
		  //˵�����ַ���������
		   return false;
		  }
		 }
		 //˵��������
		 return true;
		}
    </script>
		<script type="text/JavaScript">
		function MM_swapImgRestore() { //v3.0
		  var i,x,a=document.MM_sr; for(i=0;a&&i<a.length&&(x=a[i])&&x.oSrc;i++) x.src=x.oSrc;
		}
		
		function MM_preloadImages() { //v3.0
		  var d=document; if(d.images){ if(!d.MM_p) d.MM_p=new Array();
		    var i,j=d.MM_p.length,a=MM_preloadImages.arguments; for(i=0; i<a.length; i++)
		    if (a[i].indexOf("#")!=0){ d.MM_p[j]=new Image; d.MM_p[j++].src=a[i];}}
		}
		
		function MM_findObj(n, d) { //v4.01
		  var p,i,x;  if(!d) d=document; if((p=n.indexOf("?"))>0&&parent.frames.length) {
		    d=parent.frames[n.substring(p+1)].document; n=n.substring(0,p);}
		  if(!(x=d[n])&&d.all) x=d.all[n]; for (i=0;!x&&i<d.forms.length;i++) x=d.forms[i][n];
		  for(i=0;!x&&d.layers&&i<d.layers.length;i++) x=MM_findObj(n,d.layers[i].document);
		  if(!x && d.getElementById) x=d.getElementById(n); return x;
		}
		
		function MM_swapImage() { //v3.0
		  var i,j=0,x,a=MM_swapImage.arguments; document.MM_sr=new Array; for(i=0;i<(a.length-2);i+=3)
		   if ((x=MM_findObj(a[i]))!=null){document.MM_sr[j++]=x; if(!x.oSrc) x.oSrc=x.src; x.src=a[i+2];}
		}
		
		</script>
		<link href="/smartcardweb/pages/css/css.css" rel="stylesheet"
			type="text/css" />
		<style type="text/css">
			.��ʽ12 {
			font-family: "����"; 
			color: #000000; 
			font-size: 14px;
			}
			.��ʽ11 {
			font-family: "����"; 
			color: #000000; 
			font-size: 16px;
			font-weight: bold;
			}
			.��ʽ10 {
			font-family: "����"; 
			color: #606060; 
			font-size: 20px;
			font-weight: bold;			
			}
			.��ʽ9 {
			font-family: "����"; 
			color: #ff0000; 
			font-size: 18px;
			}
			
		</style>
	</head>
	<body scroll="no" >
		<form action="/smartcardweb/checktransfer.do" method="post"
			name="putinForm">
			<div id="mid_top"></div>
			<div id="middle">
				<div id="Layer1">
					<div id="login">
						<div id="login_left"></div>
						<div id="login_middle">
							<div align="center">
								<img src="/smartcardweb/pages/images/input-money.gif" width="343"
									height="57" />
							</div>
							<div>
								<ul>
									<li
										onmouseup="this.style.background='url(/smartcardweb/pages/images/0.gif)'"
										onmousedown="this.style.background='url(/smartcardweb/pages/images/0a.gif)'"
										style="cursor:hand; background:url(/smartcardweb/pages/images/0.gif)"
										onclick="javascript:addnum(0)"></li>
									<li
										onmouseup="this.style.background='url(/smartcardweb/pages/images/1.gif)'"
										onmousedown="this.style.background='url(/smartcardweb/pages/images/1a.gif)'"
										style="cursor:hand; background:url(/smartcardweb/pages/images/1.gif)"
										onclick="javascript:addnum(1)"></li>
									<li
										onmouseup="this.style.background='url(/smartcardweb/pages/images/2.gif)'"
										onmousedown="this.style.background='url(/smartcardweb/pages/images/2a.gif)'"
										style="cursor:hand; background:url(/smartcardweb/pages/images/2.gif)"
										onclick="javascript:addnum(2)"></li>
									<li
										onmouseup="this.style.background='url(/smartcardweb/pages/images/3.gif)'"
										onmousedown="this.style.background='url(/smartcardweb/pages/images/3a.gif)'"
										style="cursor:hand; background:url(/smartcardweb/pages/images/3.gif)"
										onclick="javascript:addnum(3)"></li>
									<li
										onmouseup="this.style.background='url(/smartcardweb/pages/images/4.gif)'"
										onmousedown="this.style.background='url(/smartcardweb/pages/images/4a.gif)'"
										style="cursor:hand; background:url(/smartcardweb/pages/images/4.gif)"
										onclick="javascript:addnum(4)"></li>
									<li
										onmouseup="this.style.background='url(/smartcardweb/pages/images/5.gif)'"
										onmousedown="this.style.background='url(/smartcardweb/pages/images/5a.gif)'"
										style="cursor:hand; background:url(/smartcardweb/pages/images/5.gif)"
										onclick="javascript:addnum(5)"></li>
									<li
										onmouseup="this.style.background='url(/smartcardweb/pages/images/6.gif)'"
										onmousedown="this.style.background='url(/smartcardweb/pages/images/6a.gif)'"
										style="cursor:hand; background:url(/smartcardweb/pages/images/6.gif)"
										onclick="javascript:addnum(6)"></li>
									<li
										onmouseup="this.style.background='url(/smartcardweb/pages/images/7.gif)'"
										onmousedown="this.style.background='url(/smartcardweb/pages/images/7a.gif)'"
										style="cursor:hand; background:url(/smartcardweb/pages/images/7.gif)"
										onclick="javascript:addnum(7)"></li>
									<li
										onmouseup="this.style.background='url(/smartcardweb/pages/images/8.gif)'"
										onmousedown="this.style.background='url(/smartcardweb/pages/images/8a.gif)'"
										style="cursor:hand; background:url(/smartcardweb/pages/images/8.gif)"
										onclick="javascript:addnum(8)"></li>
									<li
										onmouseup="this.style.background='url(/smartcardweb/pages/images/9.gif)'"
										onmousedown="this.style.background='url(/smartcardweb/pages/images/9a.gif)'"
										style="cursor:hand; background:url(/smartcardweb/pages/images/9.gif)"
										onclick="javascript:addnum(9)"></li>
									<li
										onmouseup="this.style.background='url(/smartcardweb/pages/images/del.gif)'"
										onmousedown="this.style.background='url(/smartcardweb/pages/images/dela.gif)'"
										style="cursor:hand; background:url(/smartcardweb/pages/images/del.gif)"
										onclick="javascript:cancelnum()"></li>
								</ul>
							</div>
							<div id="login_input">
								<table width="410" border="0" cellpadding="0" cellspacing="0">
									<tr>
										<td width="206" align="right">
											&nbsp;
											
										</td>
										<td width="214">
											&nbsp;
										</td>
									</tr>
									<tr>
										<td align="right">
											<img src="/smartcardweb/pages/images/money2.gif"
												width="114" height="24" />
										</td>
										<td> 
										    
										    <input type="hidden" name="nowinput" value="1" />
											<input name="money" type="text" class="��ʽ11"
												onfocus="javascript:putinForm.nowinput.value='1'" size="20"
												maxlength="4" />
										</td>
									</tr>
									<tr>
										<td class="info" colspan="2">
											<html:errors />
											&nbsp;
										</td>
									</tr>
									<tr>
										<td colspan="2" valign="bottom" class="��ʽ9">
											&nbsp;
										</td>
									</tr>
								</table>
							</div>
							<div id="login_buttom">
								<table>
									<tr>
										<td class="info">
											&nbsp;
										</td>
										<td class="info">
											&nbsp;
										</td>
									</tr>
									<tr>
										<td align="center">
											<a onclick="javascript:check();"
												onmouseup="MM_swapImgRestore()"
												onmousedown="MM_swapImage('send','','/smartcardweb/pages/images/senda.gif',1)"><img
													src="/smartcardweb/pages/images/send.gif" name="send"
													width="109" height="48" border="0" id="send" /> </a>
										</td>
										<td>
											<a
												onclick="javascript: window.location='/smartcardweb/index.do';"
												onmouseup="MM_swapImgRestore()"
												onmousedown="MM_swapImage('cancel','','/smartcardweb/pages/images/cancela.gif',1)"><img
													src="/smartcardweb/pages/images/cancel.gif" name="cancel"
													width="109" height="48" border="0" id="cancel" /> </a>
										</td>
									</tr>
								</table>
							</div>
						</div>
						<div id="login_right"></div>
					</div>
				</div>
			</div>
			<div id="bottom"></div>
			<object id="RFScanner"
				classid="clsid:{335AEB92-0D39-4999-A2B5-536A2C718901}" width="0"
				height="0" />
		</form>
	</body>
</html>