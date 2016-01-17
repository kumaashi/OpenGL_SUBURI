
const $Object=global.Object;
const $Array=global.Array;
const $String=global.String;
const $Number=global.Number;
const $Function=global.Function;
const $Boolean=global.Boolean;
const $NaN=0/0;



function EQUALS(a){
	if((typeof(this)==='string')&&(typeof(a)==='string'))return %StringEquals(this,a);
	var b=this;




	while(true){
		if((typeof(b)==='number')){
			if(a==null)return 1;
			return %NumberEquals(b,%ToNumber(a));
		}else if((typeof(b)==='string')){
			if((typeof(a)==='string'))return %StringEquals(b,a);
			if((typeof(a)==='number'))return %NumberEquals(%ToNumber(b),a);
			if((typeof(a)==='boolean'))return %NumberEquals(%ToNumber(b),%ToNumber(a));
			if(a==null)return 1;
			a=%ToPrimitive(a,0);
		}else if((typeof(b)==='boolean')){
			if((typeof(a)==='boolean')){
				return %_ObjectEquals(b,a)?0:1;
			}
			if(a==null)return 1;
			return %NumberEquals(%ToNumber(b),%ToNumber(a));
		}else if(b==null){

			return(a==null)?0:1;
		}else{

			if(a==null)return 1;
			if((%_IsSpecObject(a))){
				return %_ObjectEquals(b,a)?0:1;
			}

			b=%ToPrimitive(b,0);
		}
	}
}


function STRICT_EQUALS(a){
	if((typeof(this)==='string')){
		if(!(typeof(a)==='string'))return 1;
		return %StringEquals(this,a);
	}

	if((typeof(this)==='number')){
		if(!(typeof(a)==='number'))return 1;
		return %NumberEquals(this,a);
	}




	return %_ObjectEquals(this,a)?0:1;
}




function COMPARE(a,b){
	var c;
	var d;

	if((typeof(this)==='string')){
		if((typeof(a)==='string'))return %_StringCompare(this,a);
		if((typeof(a)==='undefined'))return b;
		c=this;
	}else if((typeof(this)==='number')){
		if((typeof(a)==='number'))return %NumberCompare(this,a,b);
		if((typeof(a)==='undefined'))return b;
		c=this;
	}else if((typeof(this)==='undefined')){
		if(!(typeof(a)==='undefined')){
			%ToPrimitive(a,1);
		}
		return b;
	}else if((typeof(a)==='undefined')){
		%ToPrimitive(this,1);
		return b;
	}else{
		c=%ToPrimitive(this,1);
	}

	d=%ToPrimitive(a,1);
	if((typeof(c)==='string')&&(typeof(d)==='string')){
		return %_StringCompare(c,d);
	}else{
		var e=%ToNumber(c);
		var f=%ToNumber(d);
		if((!%_IsSmi(%IS_VAR(e))&&!(e==e))||(!%_IsSmi(%IS_VAR(f))&&!(f==f)))return b;
		return %NumberCompare(e,f,b);
	}
}









function ADD(a){

	if((typeof(this)==='number')&&(typeof(a)==='number'))return %NumberAdd(this,a);
	if((typeof(this)==='string')&&(typeof(a)==='string'))return %_StringAdd(this,a);


	var b=%ToPrimitive(this,0);
	var c=%ToPrimitive(a,0);

	if((typeof(b)==='string')){
		return %_StringAdd(b,%ToString(c));
	}else if((typeof(c)==='string')){
		return %_StringAdd(%ToString(b),c);
	}else{
		return %NumberAdd(%ToNumber(b),%ToNumber(c));
	}
}



function STRING_ADD_LEFT(a){
	if(!(typeof(a)==='string')){
		if((%_ClassOf(a)==='String')&&%_IsStringWrapperSafeForDefaultValueOf(a)){
			a=%_ValueOf(a);
		}else{
			a=(typeof(a)==='number')
				?%_NumberToString(a)
				:%ToString(%ToPrimitive(a,0));
		}
	}
	return %_StringAdd(this,a);
}



function STRING_ADD_RIGHT(a){
	var b=this;
	if(!(typeof(b)==='string')){
		if((%_ClassOf(b)==='String')&&%_IsStringWrapperSafeForDefaultValueOf(b)){
			b=%_ValueOf(b);
		}else{
			b=(typeof(b)==='number')
				?%_NumberToString(b)
				:%ToString(%ToPrimitive(b,0));
		}
	}
	return %_StringAdd(b,a);
}



function SUB(a){
	var b=(typeof(this)==='number')?this:%ToNumber(this);
	if(!(typeof(a)==='number'))a=%ToNumber(a);
	return %NumberSub(b,a);
}



function MUL(a){
	var b=(typeof(this)==='number')?this:%ToNumber(this);
	if(!(typeof(a)==='number'))a=%ToNumber(a);
	return %NumberMul(b,a);
}



function DIV(a){
	var b=(typeof(this)==='number')?this:%ToNumber(this);
	if(!(typeof(a)==='number'))a=%ToNumber(a);
	return %NumberDiv(b,a);
}



function MOD(a){
	var b=(typeof(this)==='number')?this:%ToNumber(this);
	if(!(typeof(a)==='number'))a=%ToNumber(a);
	return %NumberMod(b,a);
}









function BIT_OR(a){
	var b=(typeof(this)==='number')?this:%ToNumber(this);
	if(!(typeof(a)==='number'))a=%ToNumber(a);
	return %NumberOr(b,a);
}



function BIT_AND(a){
	var b;
	if((typeof(this)==='number')){
		b=this;
		if(!(typeof(a)==='number'))a=%ToNumber(a);
	}else{
		b=%ToNumber(this);




		if(!(typeof(a)==='number'))a=%ToNumber(a);



		if((!%_IsSmi(%IS_VAR(b))&&!(b==b)))return 0;
	}
	return %NumberAnd(b,a);
}



function BIT_XOR(a){
	var b=(typeof(this)==='number')?this:%ToNumber(this);
	if(!(typeof(a)==='number'))a=%ToNumber(a);
	return %NumberXor(b,a);
}



function UNARY_MINUS(){
	var a=(typeof(this)==='number')?this:%ToNumber(this);
	return %NumberUnaryMinus(a);
}



function BIT_NOT(){
	var a=(typeof(this)==='number')?this:%ToNumber(this);
	return %NumberNot(a);
}



function SHL(a){
	var b=(typeof(this)==='number')?this:%ToNumber(this);
	if(!(typeof(a)==='number'))a=%ToNumber(a);
	return %NumberShl(b,a);
}



function SAR(a){
	var b;
	if((typeof(this)==='number')){
		b=this;
		if(!(typeof(a)==='number'))a=%ToNumber(a);
	}else{
		b=%ToNumber(this);




		if(!(typeof(a)==='number'))a=%ToNumber(a);



		if((!%_IsSmi(%IS_VAR(b))&&!(b==b)))return 0;
	}
	return %NumberSar(b,a);
}



function SHR(a){
	var b=(typeof(this)==='number')?this:%ToNumber(this);
	if(!(typeof(a)==='number'))a=%ToNumber(a);
	return %NumberShr(b,a);
}









function DELETE(a){
	return %DeleteProperty(%ToObject(this),%ToString(a));
}



function IN(a){
	if(!(%_IsSpecObject(a))){
		throw %MakeTypeError('invalid_in_operator_use',[this,a]);
	}
	return %_IsNonNegativeSmi(this)?%HasElement(a,this):%HasProperty(a,%ToString(this));
}






function INSTANCE_OF(a){
	var b=this;
	if(!(%_IsFunction(a))){
		throw %MakeTypeError('instanceof_function_expected',[b]);
	}


	if(!(%_IsSpecObject(b))){
		return 1;
	}


	var c=a.prototype;
	if(!(%_IsSpecObject(c))){
		throw %MakeTypeError('instanceof_nonobject_proto',[c]);
	}


	return %IsInPrototypeChain(c,b)?0:1;
}




function GET_KEYS(){
	return %GetPropertyNames(this);
}





function FILTER_KEY(a){
	var b=%ToString(a);
	if(%HasProperty(this,b))return b;
	return 0;
}


function CALL_NON_FUNCTION(){
	var a=%GetFunctionDelegate(this);
	if(!(%_IsFunction(a))){
		throw %MakeTypeError('called_non_callable',[typeof this]);
	}
	return a.apply(this,arguments);
}


function CALL_NON_FUNCTION_AS_CONSTRUCTOR(){
	var a=%GetConstructorDelegate(this);
	if(!(%_IsFunction(a))){
		throw %MakeTypeError('called_non_callable',[typeof this]);
	}
	return a.apply(this,arguments);
}


function APPLY_PREPARE(a){
	var b;



	if((%_IsArray(a))){
		b=a.length;
		if(%_IsSmi(b)&&b>=0&&b<0x800000&&(%_IsFunction(this))){
			return b;
		}
	}

	b=(a==null)?0:%ToUint32(a.length);




	if(b>0x800000){
		throw %MakeRangeError('stack_overflow',[]);
	}

	if(!(%_IsFunction(this))){
		throw %MakeTypeError('apply_non_function',[%ToString(this),typeof this]);
	}


	if(a!=null&&!(%_IsArray(a))&&!(%_ClassOf(a)==='Arguments')){
		throw %MakeTypeError('apply_wrong_args',[]);
	}



	return b;
}


function APPLY_OVERFLOW(a){
	throw %MakeRangeError('stack_overflow',[]);
}



function TO_OBJECT(){
	return %ToObject(this);
}



function TO_NUMBER(){
	return %ToNumber(this);
}



function TO_STRING(){
	return %ToString(this);
}









function ToPrimitive(a,b){

	if((typeof(a)==='string'))return a;

	if(!(%_IsSpecObject(a)))return a;
	if(b==0)b=((%_ClassOf(a)==='Date'))?2:1;
	return(b==1)?%DefaultNumber(a):%DefaultString(a);
}



function ToBoolean(a){
	if((typeof(a)==='boolean'))return a;
	if((typeof(a)==='string'))return a.length!=0;
	if(a==null)return false;
	if((typeof(a)==='number'))return!((a==0)||(!%_IsSmi(%IS_VAR(a))&&!(a==a)));
	return true;
}



function ToNumber(a){
	if((typeof(a)==='number'))return a;
	if((typeof(a)==='string')){
		return %_HasCachedArrayIndex(a)?%_GetCachedArrayIndex(a)
			:%StringToNumber(a);
	}
	if((typeof(a)==='boolean'))return a?1:0;
	if((typeof(a)==='undefined'))return $NaN;
	return((a===null))?0:ToNumber(%DefaultNumber(a));
}



function ToString(a){
	if((typeof(a)==='string'))return a;
	if((typeof(a)==='number'))return %_NumberToString(a);
	if((typeof(a)==='boolean'))return a?'true':'false';
	if((typeof(a)==='undefined'))return'undefined';
	return((a===null))?'null':%ToString(%DefaultString(a));
}

function NonStringToString(a){
	if((typeof(a)==='number'))return %_NumberToString(a);
	if((typeof(a)==='boolean'))return a?'true':'false';
	if((typeof(a)==='undefined'))return'undefined';
	return((a===null))?'null':%ToString(%DefaultString(a));
}



function ToObject(a){
	if((typeof(a)==='string'))return new $String(a);
	if((typeof(a)==='number'))return new $Number(a);
	if((typeof(a)==='boolean'))return new $Boolean(a);
	if((a==null)&&!(%_IsUndetectableObject(a))){
		throw %MakeTypeError('null_to_object',[]);
	}
	return a;
}



function ToInteger(a){
	if(%_IsSmi(a))return a;
	return %NumberToInteger(ToNumber(a));
}



function ToUint32(a){
	if(%_IsSmi(a)&&a>=0)return a;
	return %NumberToJSUint32(ToNumber(a));
}



function ToInt32(a){
	if(%_IsSmi(a))return a;
	return %NumberToJSInt32(ToNumber(a));
}



function SameValue(a,b){
	if(typeof a!=typeof b)return false;
	if((typeof(a)==='number')){
		if((!%_IsSmi(%IS_VAR(a))&&!(a==a))&&(!%_IsSmi(%IS_VAR(b))&&!(b==b)))return true;

		if(a===0&&b===0&&(1/a)!=(1/b)){
			return false;
		}
		return a===b;
	}
	return a===b
}









function IsPrimitive(a){



	return!(%_IsSpecObject(a));
}



function DefaultNumber(a){
	if((%_IsFunction(a.valueOf))){
		var b=a.valueOf();
		if(%IsPrimitive(b))return b;
	}

	if((%_IsFunction(a.toString))){
		var c=a.toString();
		if(%IsPrimitive(c))return c;
	}

	throw %MakeTypeError('cannot_convert_to_primitive',[]);
}



function DefaultString(a){
	if((%_IsFunction(a.toString))){
		var b=a.toString();
		if(%IsPrimitive(b))return b;
	}

	if((%_IsFunction(a.valueOf))){
		var c=a.valueOf();
		if(%IsPrimitive(c))return c;
	}

	throw %MakeTypeError('cannot_convert_to_primitive',[]);
}







%FunctionSetPrototype($Array,new $Array(0));









































const $isNaN=GlobalIsNaN;
const $isFinite=GlobalIsFinite;






function InstallFunctions(a,b,c){
	if(c.length>=8){
		%OptimizeObjectForAddingMultipleProperties(a,c.length>>1);
	}
	for(var d=0;d<c.length;d+=2){
		var e=c[d];
		var f=c[d+1];
		%FunctionSetName(f,e);
		%FunctionRemovePrototype(f);
		%SetProperty(a,e,f,b);
	}
	%ToFastProperties(a);
}





function InstallFunctionsOnHiddenPrototype(a,b,c){
	var d=new $Object();
	%SetHiddenPrototype(a,d);
	InstallFunctions(d,b,c);
}






function GlobalIsNaN(a){
	var b=ToNumber(a);
	return(!%_IsSmi(%IS_VAR(b))&&!(b==b));
}



function GlobalIsFinite(a){
	if(!(typeof(a)==='number'))a=ToNumber(a);


	return %_IsSmi(a)||a-a==0;
}



function GlobalParseInt(a,b){
	if((typeof(b)==='undefined')){





		if(%_IsSmi(a))return a;
		if((typeof(a)==='number')&&
				((0.01<a&&a<1e9)||
				 (-1e9<a&&a<-0.01))){

					 return a|0;
				 }
		b=0;
	}else{
		b=(%_IsSmi(%IS_VAR(b))?b:(b>>0));
		if(!(b==0||(2<=b&&b<=36)))
			return $NaN;
	}
	a=((typeof(%IS_VAR(a))==='string')?a:NonStringToString(a));
	if(%_HasCachedArrayIndex(a)&&
			(b==0||b==10)){
				return %_GetCachedArrayIndex(a);
			}
	return %StringParseInt(a,b);
}



function GlobalParseFloat(a){
	a=((typeof(%IS_VAR(a))==='string')?a:NonStringToString(a));
	if(%_HasCachedArrayIndex(a))return %_GetCachedArrayIndex(a);
	return %StringParseFloat(a);
}


function GlobalEval(a){
	if(!(typeof(a)==='string'))return a;

	var b=%GlobalReceiver(global);
	var c=(this===b);
	var d=(global===b);

	if(!c||d){
		throw new $EvalError('The "this" object passed to eval must '+
				'be the global object from which eval originated');
	}

	var e=%CompileString(a,false);
	if(!(%_IsFunction(e)))return e;

	return e.call(this);
}



function GlobalExecScript(a,b){

	if(!b||/javascript/i.test(b)){
		var c=%CompileString(ToString(a),false);
		c.call(%GlobalReceiver(global));
	}
	return null;
}





function SetupGlobal(){

	%SetProperty(global,"NaN",$NaN,2|4);


	%SetProperty(global,"Infinity",1/0,2|4);


	%SetProperty(global,"undefined",void 0,2|4);


	InstallFunctions(global,2,$Array(
				"isNaN",GlobalIsNaN,
				"isFinite",GlobalIsFinite,
				"parseInt",GlobalParseInt,
				"parseFloat",GlobalParseFloat,
				"eval",GlobalEval,
				"execScript",GlobalExecScript
				));
}

SetupGlobal();






%SetCode($Boolean,function(a){
	if(%_IsConstructCall()){
		%_SetValueOf(this,ToBoolean(a));
	}else{
		return ToBoolean(a);
	}
});

%FunctionSetPrototype($Boolean,new $Boolean(false));

%SetProperty($Boolean.prototype,"constructor",$Boolean,2);




$Object.prototype.constructor=$Object;


function ObjectToString(){
	return"[object "+%_ClassOf(ToObject(this))+"]";
}



function ObjectToLocaleString(){
	return this.toString();
}



function ObjectValueOf(){
	return ToObject(this);
}



function ObjectHasOwnProperty(a){
	return %HasLocalProperty(ToObject(this),ToString(a));
}



function ObjectIsPrototypeOf(a){
	if(!(%_IsSpecObject(a)))return false;
	return %IsInPrototypeChain(this,a);
}



function ObjectPropertyIsEnumerable(a){
	return %IsPropertyEnumerable(ToObject(this),ToString(a));
}



function ObjectDefineGetter(a,b){
	if(this==null&&!(%_IsUndetectableObject(this))){
		throw new $TypeError('Object.prototype.__defineGetter__: this is Null');
	}
	if(!(%_IsFunction(b))){
		throw new $TypeError('Object.prototype.__defineGetter__: Expecting function');
	}
	return %DefineAccessor(ToObject(this),ToString(a),0,b);
}


function ObjectLookupGetter(a){
	if(this==null&&!(%_IsUndetectableObject(this))){
		throw new $TypeError('Object.prototype.__lookupGetter__: this is Null');
	}
	return %LookupAccessor(ToObject(this),ToString(a),0);
}


function ObjectDefineSetter(a,b){
	if(this==null&&!(%_IsUndetectableObject(this))){
		throw new $TypeError('Object.prototype.__defineSetter__: this is Null');
	}
	if(!(%_IsFunction(b))){
		throw new $TypeError(
				'Object.prototype.__defineSetter__: Expecting function');
	}
	return %DefineAccessor(ToObject(this),ToString(a),1,b);
}


function ObjectLookupSetter(a){
	if(this==null&&!(%_IsUndetectableObject(this))){
		throw new $TypeError('Object.prototype.__lookupSetter__: this is Null');
	}
	return %LookupAccessor(ToObject(this),ToString(a),1);
}


function ObjectKeys(a){
	if(!(%_IsSpecObject(a)))
		throw MakeTypeError("obj_ctor_property_non_object",["keys"]);
	return %LocalKeys(a);
}



function IsAccessorDescriptor(a){
	if((typeof(a)==='undefined'))return false;
	return a.hasGetter_||a.hasSetter_;
}



function IsDataDescriptor(a){
	if((typeof(a)==='undefined'))return false;
	return a.hasValue_||a.hasWritable_;
}



function IsGenericDescriptor(a){
	return!(IsAccessorDescriptor(a)||IsDataDescriptor(a));
}


function IsInconsistentDescriptor(a){
	return IsAccessorDescriptor(a)&&IsDataDescriptor(a);
}


function FromPropertyDescriptor(a){
	if((typeof(a)==='undefined'))return a;
	var b=new $Object();
	if(IsDataDescriptor(a)){
		b.value=a.getValue();
		b.writable=a.isWritable();
	}
	if(IsAccessorDescriptor(a)){
		b.get=a.getGet();
		b.set=a.getSet();
	}
	b.enumerable=a.isEnumerable();
	b.configurable=a.isConfigurable();
	return b;
}


function ToPropertyDescriptor(a){
	if(!(%_IsSpecObject(a))){
		throw MakeTypeError("property_desc_object",[a]);
	}
	var b=new PropertyDescriptor();

	if("enumerable"in a){
		b.setEnumerable(ToBoolean(a.enumerable));
	}

	if("configurable"in a){
		b.setConfigurable(ToBoolean(a.configurable));
	}

	if("value"in a){
		b.setValue(a.value);
	}

	if("writable"in a){
		b.setWritable(ToBoolean(a.writable));
	}

	if("get"in a){
		var c=a.get;
		if(!(typeof(c)==='undefined')&&!(%_IsFunction(c))){
			throw MakeTypeError("getter_must_be_callable",[c]);
		}
		b.setGet(c);
	}

	if("set"in a){
		var d=a.set;
		if(!(typeof(d)==='undefined')&&!(%_IsFunction(d))){
			throw MakeTypeError("setter_must_be_callable",[d]);
		}
		b.setSet(d);
	}

	if(IsInconsistentDescriptor(b)){
		throw MakeTypeError("value_and_accessor",[a]);
	}
	return b;
}


function PropertyDescriptor(){


	this.value_=void 0;
	this.hasValue_=false;
	this.writable_=false;
	this.hasWritable_=false;
	this.enumerable_=false;
	this.hasEnumerable_=false;
	this.configurable_=false;
	this.hasConfigurable_=false;
	this.get_=void 0;
	this.hasGetter_=false;
	this.set_=void 0;
	this.hasSetter_=false;
}


PropertyDescriptor.prototype.setValue=function(a){
	this.value_=a;
	this.hasValue_=true;
}


PropertyDescriptor.prototype.getValue=function(){
	return this.value_;
}


PropertyDescriptor.prototype.hasValue=function(){
	return this.hasValue_;
}


PropertyDescriptor.prototype.setEnumerable=function(a){
	this.enumerable_=a;
	this.hasEnumerable_=true;
}


PropertyDescriptor.prototype.isEnumerable=function(){
	return this.enumerable_;
}


PropertyDescriptor.prototype.hasEnumerable=function(){
	return this.hasEnumerable_;
}


PropertyDescriptor.prototype.setWritable=function(a){
	this.writable_=a;
	this.hasWritable_=true;
}


PropertyDescriptor.prototype.isWritable=function(){
	return this.writable_;
}


PropertyDescriptor.prototype.hasWritable=function(){
	return this.hasWritable_;
}


PropertyDescriptor.prototype.setConfigurable=function(a){
	this.configurable_=a;
	this.hasConfigurable_=true;
}


PropertyDescriptor.prototype.hasConfigurable=function(){
	return this.hasConfigurable_;
}


PropertyDescriptor.prototype.isConfigurable=function(){
	return this.configurable_;
}


PropertyDescriptor.prototype.setGet=function(a){
	this.get_=a;
	this.hasGetter_=true;
}


PropertyDescriptor.prototype.getGet=function(){
	return this.get_;
}


PropertyDescriptor.prototype.hasGetter=function(){
	return this.hasGetter_;
}


PropertyDescriptor.prototype.setSet=function(a){
	this.set_=a;
	this.hasSetter_=true;
}


PropertyDescriptor.prototype.getSet=function(){
	return this.set_;
}


PropertyDescriptor.prototype.hasSetter=function(){
	return this.hasSetter_;
}





function ConvertDescriptorArrayToDescriptor(a){
	if(a==false){
		throw'Internal error: invalid desc_array';
	}

	if((typeof(a)==='undefined')){
		return void 0;
	}

	var b=new PropertyDescriptor();

	if(a[0]){
		b.setGet(a[2]);
		b.setSet(a[3]);
	}else{
		b.setValue(a[1]);
		b.setWritable(a[4]);
	}
	b.setEnumerable(a[5]);
	b.setConfigurable(a[6]);

	return b;
}



function GetProperty(a,b){
	var c=GetOwnProperty(a);
	if(!(typeof(c)==='undefined'))return c;
	var d=a.__proto__;
	if((d===null))return void 0;
	return GetProperty(d,b);
}



function HasProperty(a,b){
	var c=GetProperty(a,b);
	return(typeof(c)==='undefined')?false:true;
}



function GetOwnProperty(a,b){



	var c=%GetOwnProperty(ToObject(a),ToString(b));


	if(c==false)return void 0;

	return ConvertDescriptorArrayToDescriptor(c);
}



function DefineOwnProperty(a,b,c,d){
	var e=%GetOwnProperty(ToObject(a),ToString(b));

	if(e==false)return void 0;

	var f=ConvertDescriptorArrayToDescriptor(e);
	var g=%IsExtensible(ToObject(a));



	if((typeof(f)==='undefined')&&!g)
		throw MakeTypeError("define_disallowed",["defineProperty"]);

	if(!(typeof(f)==='undefined')&&!f.isConfigurable()){

		if((!c.hasEnumerable()||
					SameValue(c.isEnumerable()&&f.isEnumerable()))&&
				(!c.hasConfigurable()||
				 SameValue(c.isConfigurable(),f.isConfigurable()))&&
				(!c.hasWritable()||
				 SameValue(c.isWritable(),f.isWritable()))&&
				(!c.hasValue()||
				 SameValue(c.getValue(),f.getValue()))&&
				(!c.hasGetter()||
				 SameValue(c.getGet(),f.getGet()))&&
				(!c.hasSetter()||
				 SameValue(c.getSet(),f.getSet()))){
					 return true;
				 }


		if(c.isConfigurable()||c.isEnumerable()!=f.isEnumerable())
			throw MakeTypeError("redefine_disallowed",["defineProperty"]);

		if(IsDataDescriptor(f)!=IsDataDescriptor(c))
			throw MakeTypeError("redefine_disallowed",["defineProperty"]);

		if(IsDataDescriptor(f)&&IsDataDescriptor(c)){
			if(!f.isWritable()&&c.isWritable())
				throw MakeTypeError("redefine_disallowed",["defineProperty"]);
			if(!f.isWritable()&&c.hasValue()&&
					!SameValue(c.getValue(),f.getValue())){
						throw MakeTypeError("redefine_disallowed",["defineProperty"]);
					}
		}

		if(IsAccessorDescriptor(c)&&IsAccessorDescriptor(f)){
			if(c.hasSetter()&&!SameValue(c.getSet(),f.getSet())){
				throw MakeTypeError("redefine_disallowed",["defineProperty"]);
			}
			if(c.hasGetter()&&!SameValue(c.getGet(),f.getGet()))
				throw MakeTypeError("redefine_disallowed",["defineProperty"]);
		}
	}





	var h=0;
	if(c.hasEnumerable()){
		h|=c.isEnumerable()?0:2;
	}else if(!(typeof(f)==='undefined')){
		h|=f.isEnumerable()?0:2;
	}else{
		h|=2;
	}

	if(c.hasConfigurable()){
		h|=c.isConfigurable()?0:4;
	}else if(!(typeof(f)==='undefined')){
		h|=f.isConfigurable()?0:4;
	}else
		h|=4;

	if(IsDataDescriptor(c)||IsGenericDescriptor(c)){
		if(c.hasWritable()){
			h|=c.isWritable()?0:1;
		}else if(!(typeof(f)==='undefined')){
			h|=f.isWritable()?0:1;
		}else{
			h|=1;
		}
		%DefineOrRedefineDataProperty(a,b,c.getValue(),h);
	}else{
		if(c.hasGetter()&&(%_IsFunction(c.getGet()))){
			%DefineOrRedefineAccessorProperty(a,b,0,c.getGet(),h);
		}
		if(c.hasSetter()&&(%_IsFunction(c.getSet()))){
			%DefineOrRedefineAccessorProperty(a,b,1,c.getSet(),h);
		}
	}
	return true;
}



function ObjectGetPrototypeOf(a){
	if(!(%_IsSpecObject(a)))
		throw MakeTypeError("obj_ctor_property_non_object",["getPrototypeOf"]);
	return a.__proto__;
}



function ObjectGetOwnPropertyDescriptor(a,b){
	if(!(%_IsSpecObject(a)))
		throw MakeTypeError("obj_ctor_property_non_object",["getOwnPropertyDescriptor"]);
	var c=GetOwnProperty(a,b);
	return FromPropertyDescriptor(c);
}



function ObjectGetOwnPropertyNames(a){
	if(!(%_IsSpecObject(a)))
		throw MakeTypeError("obj_ctor_property_non_object",["getOwnPropertyNames"]);




	var b=%GetLocalElementNames(a);


	if(%GetInterceptorInfo(a)&1){
		var c=
			%GetIndexedInterceptorElementNames(a);
		if(c)
			b=b.concat(c);
	}




	b=b.concat(%GetLocalPropertyNames(a));



	if(%GetInterceptorInfo(a)&2){
		var d=
			%GetNamedInterceptorPropertyNames(a);
		if(d){
			b=b.concat(d);
		}
	}


	var e={};
	var f=0;
	for(var g=0;g<b.length;++g){
		var h=ToString(b[g]);



		if(e[h]===true)
			continue;
		e[h]=true;
		b[f++]=h;
	}
	b.length=f;

	return b;
}



function ObjectCreate(a,b){
	if(!(%_IsSpecObject(a))&&a!==null){
		throw MakeTypeError("proto_object_or_null",[a]);
	}
	var c=new $Object();
	c.__proto__=a;
	if(!(typeof(b)==='undefined'))ObjectDefineProperties(c,b);
	return c;
}



function ObjectDefineProperty(a,b,c){
	if(!(%_IsSpecObject(a))){
		throw MakeTypeError("obj_ctor_property_non_object",["defineProperty"]);
	}
	var d=ToString(b);
	var e=ToPropertyDescriptor(c);
	DefineOwnProperty(a,d,e,true);
	return a;
}



function ObjectDefineProperties(a,b){
	if(!(%_IsSpecObject(a)))
		throw MakeTypeError("obj_ctor_property_non_object",["defineProperties"]);
	var c=ToObject(b);
	var d=[];
	for(var e in c){
		if(%HasLocalProperty(c,e)){
			d.push(e);
			var f=c[e];
			var g=ToPropertyDescriptor(f);
			d.push(g);
		}
	}
	for(var h=0;h<d.length;h+=2){
		var e=d[h];
		var g=d[h+1];
		DefineOwnProperty(a,e,g,true);
	}
	return a;
}



function ObjectSeal(a){
	if(!(%_IsSpecObject(a))){
		throw MakeTypeError("obj_ctor_property_non_object",["seal"]);
	}
	var b=ObjectGetOwnPropertyNames(a);
	for(var c=0;c<b.length;c++){
		var d=b[c];
		var e=GetOwnProperty(a,d);
		if(e.isConfigurable())e.setConfigurable(false);
		DefineOwnProperty(a,d,e,true);
	}
	return ObjectPreventExtension(a);
}



function ObjectFreeze(a){
	if(!(%_IsSpecObject(a))){
		throw MakeTypeError("obj_ctor_property_non_object",["freeze"]);
	}
	var b=ObjectGetOwnPropertyNames(a);
	for(var c=0;c<b.length;c++){
		var d=b[c];
		var e=GetOwnProperty(a,d);
		if(IsDataDescriptor(e))e.setWritable(false);
		if(e.isConfigurable())e.setConfigurable(false);
		DefineOwnProperty(a,d,e,true);
	}
	return ObjectPreventExtension(a);
}



function ObjectPreventExtension(a){
	if(!(%_IsSpecObject(a))){
		throw MakeTypeError("obj_ctor_property_non_object",["preventExtension"]);
	}
	%PreventExtensions(a);
	return a;
}



function ObjectIsSealed(a){
	if(!(%_IsSpecObject(a))){
		throw MakeTypeError("obj_ctor_property_non_object",["isSealed"]);
	}
	var b=ObjectGetOwnPropertyNames(a);
	for(var c=0;c<b.length;c++){
		var d=b[c];
		var e=GetOwnProperty(a,d);
		if(e.isConfigurable())return false;
	}
	if(!ObjectIsExtensible(a)){
		return true;
	}
	return false;
}



function ObjectIsFrozen(a){
	if(!(%_IsSpecObject(a))){
		throw MakeTypeError("obj_ctor_property_non_object",["isFrozen"]);
	}
	var b=ObjectGetOwnPropertyNames(a);
	for(var c=0;c<b.length;c++){
		var d=b[c];
		var e=GetOwnProperty(a,d);
		if(IsDataDescriptor(e)&&e.isWritable())return false;
		if(e.isConfigurable())return false;
	}
	if(!ObjectIsExtensible(a)){
		return true;
	}
	return false;
}



function ObjectIsExtensible(a){
	if(!(%_IsSpecObject(a))){
		throw MakeTypeError("obj_ctor_property_non_object",["preventExtension"]);
	}
	return %IsExtensible(a);
}


%SetCode($Object,function(a){
	if(%_IsConstructCall()){
		if(a==null)return this;
		return ToObject(a);
	}else{
		if(a==null)return{};
		return ToObject(a);
	}
});

%SetExpectedNumberOfProperties($Object,4);




function SetupObject(){

	InstallFunctions($Object.prototype,2,$Array(
				"toString",ObjectToString,
				"toLocaleString",ObjectToLocaleString,
				"valueOf",ObjectValueOf,
				"hasOwnProperty",ObjectHasOwnProperty,
				"isPrototypeOf",ObjectIsPrototypeOf,
				"propertyIsEnumerable",ObjectPropertyIsEnumerable,
				"__defineGetter__",ObjectDefineGetter,
				"__lookupGetter__",ObjectLookupGetter,
				"__defineSetter__",ObjectDefineSetter,
				"__lookupSetter__",ObjectLookupSetter
				));
	InstallFunctions($Object,2,$Array(
				"keys",ObjectKeys,
				"create",ObjectCreate,
				"defineProperty",ObjectDefineProperty,
				"defineProperties",ObjectDefineProperties,
				"freeze",ObjectFreeze,
				"getPrototypeOf",ObjectGetPrototypeOf,
				"getOwnPropertyDescriptor",ObjectGetOwnPropertyDescriptor,
				"getOwnPropertyNames",ObjectGetOwnPropertyNames,
				"isExtensible",ObjectIsExtensible,
				"isFrozen",ObjectIsFrozen,
				"isSealed",ObjectIsSealed,
				"preventExtensions",ObjectPreventExtension,
				"seal",ObjectSeal
				));
}

SetupObject();





function BooleanToString(){


	if(!(typeof(this)==='boolean')&&!(%_ClassOf(this)==='Boolean'))
		throw new $TypeError('Boolean.prototype.toString is not generic');
	return ToString(%_ValueOf(this));
}


function BooleanValueOf(){


	if(!(typeof(this)==='boolean')&&!(%_ClassOf(this)==='Boolean'))
		throw new $TypeError('Boolean.prototype.valueOf is not generic');
	return %_ValueOf(this);
}


function BooleanToJSON(a){
	return CheckJSONPrimitive(this.valueOf());
}





function SetupBoolean(){
	InstallFunctions($Boolean.prototype,2,$Array(
				"toString",BooleanToString,
				"valueOf",BooleanValueOf,
				"toJSON",BooleanToJSON
				));
}

SetupBoolean();





%SetCode($Number,function(a){
	var b=%_ArgumentsLength()==0?0:ToNumber(a);
	if(%_IsConstructCall()){
		%_SetValueOf(this,b);
	}else{
		return b;
	}
});

%FunctionSetPrototype($Number,new $Number(0));


function NumberToString(a){


	var b=this;
	if(!(typeof(this)==='number')){
		if(!(%_ClassOf(this)==='Number'))
			throw new $TypeError('Number.prototype.toString is not generic');

		b=%_ValueOf(this);
	}

	if((typeof(a)==='undefined')||a===10){
		return ToString(b);
	}


	a=(%_IsSmi(%IS_VAR(a))?a:%NumberToInteger(ToNumber(a)));
	if(a<2||a>36){
		throw new $RangeError('toString() radix argument must be between 2 and 36');
	}

	return %NumberToRadixString(b,a);
}



function NumberToLocaleString(){
	return this.toString();
}



function NumberValueOf(){


	if(!(typeof(this)==='number')&&!(%_ClassOf(this)==='Number'))
		throw new $TypeError('Number.prototype.valueOf is not generic');
	return %_ValueOf(this);
}



function NumberToFixed(a){
	var b=(%_IsSmi(%IS_VAR(a))?a:%NumberToInteger(ToNumber(a)));
	if(b<0||b>20){
		throw new $RangeError("toFixed() digits argument must be between 0 and 20");
	}
	var c=ToNumber(this);
	return %NumberToFixed(c,b);
}



function NumberToExponential(a){
	var b=-1;
	if(!(typeof(a)==='undefined')){
		b=(%_IsSmi(%IS_VAR(a))?a:%NumberToInteger(ToNumber(a)));
		if(b<0||b>20){
			throw new $RangeError("toExponential() argument must be between 0 and 20");
		}
	}
	var c=ToNumber(this);
	return %NumberToExponential(c,b);
}



function NumberToPrecision(a){
	if((typeof(a)==='undefined'))return ToString(%_ValueOf(this));
	var b=(%_IsSmi(%IS_VAR(a))?a:%NumberToInteger(ToNumber(a)));
	if(b<1||b>21){
		throw new $RangeError("toPrecision() argument must be between 1 and 21");
	}
	var c=ToNumber(this);
	return %NumberToPrecision(c,b);
}


function CheckJSONPrimitive(a){
	if(!IsPrimitive(a))
		throw MakeTypeError('result_not_primitive',['toJSON',a]);
	return a;
}


function NumberToJSON(a){
	return CheckJSONPrimitive(this.valueOf());
}




function SetupNumber(){
	%OptimizeObjectForAddingMultipleProperties($Number.prototype,8);

	%SetProperty($Number.prototype,"constructor",$Number,2);

	%OptimizeObjectForAddingMultipleProperties($Number,5);

	%SetProperty($Number,
			"MAX_VALUE",
			1.7976931348623157e+308,
			2|4|1);


	%SetProperty($Number,"MIN_VALUE",5e-324,2|4|1);


	%SetProperty($Number,"NaN",$NaN,2|4|1);


	%SetProperty($Number,
			"NEGATIVE_INFINITY",
			-1/0,
			2|4|1);


	%SetProperty($Number,
			"POSITIVE_INFINITY",
			1/0,
			2|4|1);
	%ToFastProperties($Number);


	InstallFunctions($Number.prototype,2,$Array(
				"toString",NumberToString,
				"toLocaleString",NumberToLocaleString,
				"valueOf",NumberValueOf,
				"toFixed",NumberToFixed,
				"toExponential",NumberToExponential,
				"toPrecision",NumberToPrecision,
				"toJSON",NumberToJSON
				));
}

SetupNumber();






$Function.prototype.constructor=$Function;

function FunctionSourceString(a){
	if(!(%_IsFunction(a))){
		throw new $TypeError('Function.prototype.toString is not generic');
	}

	var b=%FunctionGetSourceCode(a);
	if(!(typeof(b)==='string')||%FunctionIsBuiltin(a)){
		var c=%FunctionGetName(a);
		if(c){

			return'function '+c+'() { [native code] }';
		}else{
			return'function () { [native code] }';
		}
	}

	var c=%FunctionGetName(a);
	return'function '+c+b;
}


function FunctionToString(){
	return FunctionSourceString(this);
}



function FunctionBind(a){
	if(!(%_IsFunction(this))){
		throw new $TypeError('Bind must be called on a function');
	}

	var b=(%_ArgumentsLength()||1)-1;
	if(b>0){
		var c=new $Array(b);
		for(var d=0;d<b;d++){
			c[d]=%_Arguments(d+1);
		}
	}
	var e=this;
	var f=function(){


		var g=%_ArgumentsLength();
		var h=new $Array(g+b);

		for(var d=0;d<b;d++){
			h[d]=c[d];
		}

		for(var d=0;d<g;d++){
			h[b+d]=%_Arguments(d);
		}


		if(%_IsConstructCall()){
			return %NewObjectFromBound(e,h);
		}
		return e.apply(a,h);
	};









	var i=(this.length-b)>0?this.length-b:0;
	%FunctionSetLength(f,i);

	return f;
}


function NewFunction(a){
	var b=%_ArgumentsLength();
	var c='';
	if(b>1){
		c=new $Array(b-1);



		for(var d=0;d<b-1;d++)c[d]=ToString(%_Arguments(d));
		c=c.join(',');



		if(c.indexOf(')')!=-1)throw MakeSyntaxError('unable_to_parse',[]);
	}
	var e=(b>0)?ToString(%_Arguments(b-1)):'';
	var f='(function('+c+') {\n'+e+'\n})';



	var g=%CompileString(f,false)();
	%FunctionSetName(g,"anonymous");
	return %SetNewFunctionAttributes(g);
}

%SetCode($Function,NewFunction);



function SetupFunction(){
	InstallFunctions($Function.prototype,2,$Array(
				"bind",FunctionBind,
				"toString",FunctionToString
				));
}

SetupFunction();




































var visited_arrays=new $Array();




function GetSortedArrayKeys(a,b){
	var c=b.length;
	var d=[];
	for(var e=0;e<c;e++){
		var f=b[e];
		if(f<0){
			var g=-1-f;
			var h=g+b[++e];
			for(;g<h;g++){
				var i=a[g];
				if(!(typeof(i)==='undefined')||g in a){
					d.push(g);
				}
			}
		}else{

			if(!(typeof(f)==='undefined')){
				var i=a[f];
				if(!(typeof(i)==='undefined')||f in a){
					d.push(f);
				}
			}
		}
	}
	d.sort(function(j,k){return j-k;});
	return d;
}



function SparseJoin(a,b,c){
	var d=GetSortedArrayKeys(a,%GetArrayKeys(a,b));
	var e=-1;
	var f=d.length;

	var g=new $Array(f);
	var h=0;

	for(var i=0;i<f;i++){
		var j=d[i];
		if(j!=e){
			var k=a[j];
			if(!(typeof(k)==='string'))k=c(k);
			g[h++]=k;
			e=j;
		}
	}
	return %StringBuilderConcat(g,h,'');
}


function UseSparseVariant(a,b,c){
	return c&&
		b>1000&&
		(!%_IsSmi(b)||
		 %EstimateNumberOfElements(a)<(b>>2));
}


function Join(a,b,c,d){
	if(b==0)return'';

	var e=(%_IsArray(a));

	if(e){


		if(!%PushIfAbsent(visited_arrays,a))return'';
	}


	try{
		if(UseSparseVariant(a,b,e)&&(c.length==0)){
			return SparseJoin(a,b,d);
		}


		if(b==1){
			var f=a[0];
			if(!(typeof(f)==='undefined')||(0 in a)){
				if((typeof(f)==='string'))return f;
				return d(f);
			}
		}


		var g;
		var h=0;


		if(c.length==0){
			g=new $Array(b);
			for(var i=0;i<b;i++){
				var f=a[i];
				if(!(typeof(f)==='undefined')||(i in a)){
					if(!(typeof(f)==='string'))f=d(f);
					g[h++]=f;
				}
			}
		}else{
			g=new $Array(b<<1);
			for(var i=0;i<b;i++){
				var f=a[i];
				if(i!=0)g[h++]=c;
				if(!(typeof(f)==='undefined')||(i in a)){
					if(!(typeof(f)==='string'))f=d(f);
					g[h++]=f;
				}
			}
		}
		return %StringBuilderConcat(g,h,'');
	}finally{

		if(e)visited_arrays.pop();
	}
}


function ConvertToString(a){
	if(a==null)return'';
	else return ToString(a);
}


function ConvertToLocaleString(a){
	if(a==null){
		return'';
	}else{



		var b=ToObject(a);
		if((%_IsFunction(b.toLocaleString)))
			return ToString(b.toLocaleString());
		else
			return ToString(a);
	}
}




function SmartSlice(a,b,c,d,f){


	var g=%GetArrayKeys(a,b+c);
	var h=g.length;
	for(var i=0;i<h;i++){
		var j=g[i];
		if(j<0){
			var k=-1-j;
			var l=k+g[++i];
			if(k<b){
				k=b;
			}
			for(;k<l;k++){




				var m=a[k];
				if(!(typeof(m)==='undefined')||k in a){
					f[k-b]=m;
				}
			}
		}else{
			if(!(typeof(j)==='undefined')){
				if(j>=b){




					var m=a[j];
					if(!(typeof(m)==='undefined')||j in a){
						f[j-b]=m;
					}
				}
			}
		}
	}
}




function SmartMove(a,b,c,d,f){

	var g=new $Array(d-c+f);
	var h=%GetArrayKeys(a,d);
	var i=h.length;
	for(var j=0;j<i;j++){
		var k=h[j];
		if(k<0){
			var l=-1-k;
			var m=l+h[++j];
			while(l<b&&l<m){



				var n=a[l];
				if(!(typeof(n)==='undefined')||l in a){
					g[l]=n;
				}
				l++;
			}
			l=b+c;
			while(l<m){




				var n=a[l];
				if(!(typeof(n)==='undefined')||l in a){
					g[l-c+f]=n;
				}
				l++;
			}
		}else{
			if(!(typeof(k)==='undefined')){
				if(k<b){



					var n=a[k];
					if(!(typeof(n)==='undefined')||k in a){
						g[k]=n;
					}
				}else if(k>=b+c){




					var n=a[k];
					if(!(typeof(n)==='undefined')||k in a){
						g[k-c+f]=n;
					}
				}
			}
		}
	}

	%MoveArrayContents(g,a);
}





function SimpleSlice(a,b,c,d,f){
	for(var g=0;g<c;g++){
		var h=b+g;



		var i=a[h];
		if(!(typeof(i)==='undefined')||h in a)
			f[g]=i;
	}
}


function SimpleMove(a,b,c,d,f){
	if(f!==c){


		if(f>c){
			for(var g=d-c;g>b;g--){
				var h=g+c-1;
				var i=g+f-1;



				var j=a[h];
				if(!(typeof(j)==='undefined')||h in a){
					a[i]=j;
				}else{
					delete a[i];
				}
			}
		}else{
			for(var g=b;g<d-c;g++){
				var h=g+c;
				var i=g+f;



				var j=a[h];
				if(!(typeof(j)==='undefined')||h in a){
					a[i]=j;
				}else{
					delete a[i];
				}
			}
			for(var g=d;g>d-c+f;g--){
				delete a[g-1];
			}
		}
	}
}





function ArrayToString(){
	if(!(%_IsArray(this))){
		throw new $TypeError('Array.prototype.toString is not generic');
	}
	return Join(this,this.length,',',ConvertToString);
}


function ArrayToLocaleString(){
	if(!(%_IsArray(this))){
		throw new $TypeError('Array.prototype.toString is not generic');
	}
	return Join(this,this.length,',',ConvertToLocaleString);
}


function ArrayJoin(a){
	if((typeof(a)==='undefined')){
		a=',';
	}else if(!(typeof(a)==='string')){
		a=ToString(a);
	}
	var b=(this.length>>>0);
	return Join(this,b,a,ConvertToString);
}




function ArrayPop(){
	var a=(this.length>>>0);
	if(a==0){
		this.length=a;
		return;
	}
	a--;
	var b=this[a];
	this.length=a;
	delete this[a];
	return b;
}




function ArrayPush(){
	var a=(this.length>>>0);
	var b=%_ArgumentsLength();
	for(var c=0;c<b;c++){
		this[c+a]=%_Arguments(c);
	}
	this.length=a+b;
	return this.length;
}


function ArrayConcat(a){

	var b=%_ArgumentsLength();
	var c=new $Array(1+b);
	c[0]=this;
	for(var d=0;d<b;d++){
		c[d+1]=%_Arguments(d);
	}

	return %ArrayConcat(c);
}



function SparseReverse(a,b){
	var c=GetSortedArrayKeys(a,%GetArrayKeys(a,b));
	var d=c.length-1;
	var f=0;
	while(f<=d){
		var g=c[f];
		var h=c[d];

		var i=b-h-1;
		var j,k;

		if(i<=g){
			k=h;
			while(c[--d]==h);
			j=i;
		}
		if(i>=g){
			j=g;
			while(c[++f]==g);
			k=b-g-1;
		}

		var l=a[j];
		if(!(typeof(l)==='undefined')||j in a){
			var m=a[k];
			if(!(typeof(m)==='undefined')||k in a){
				a[j]=m;
				a[k]=l;
			}else{
				a[k]=l;
				delete a[j];
			}
		}else{
			var m=a[k];
			if(!(typeof(m)==='undefined')||k in a){
				a[j]=m;
				delete a[k];
			}
		}
	}
}


function ArrayReverse(){
	var a=(this.length>>>0)-1;

	if(UseSparseVariant(this,a,(%_IsArray(this)))){
		SparseReverse(this,a+1);
		return this;
	}

	for(var b=0;b<a;b++,a--){
		var c=this[b];
		if(!(typeof(c)==='undefined')||b in this){
			var d=this[a];
			if(!(typeof(d)==='undefined')||a in this){
				this[b]=d;
				this[a]=c;
			}else{
				this[a]=c;
				delete this[b];
			}
		}else{
			var d=this[a];
			if(!(typeof(d)==='undefined')||a in this){
				this[b]=d;
				delete this[a];
			}
		}
	}
	return this;
}


function ArrayShift(){
	var a=(this.length>>>0);

	if(a===0){
		this.length=0;
		return;
	}

	var b=this[0];

	if((%_IsArray(this)))
		SmartMove(this,0,1,a,0);
	else
		SimpleMove(this,0,1,a,0);

	this.length=a-1;

	return b;
}


function ArrayUnshift(a){
	var b=(this.length>>>0);
	var c=%_ArgumentsLength();

	if((%_IsArray(this)))
		SmartMove(this,0,0,b,c);
	else
		SimpleMove(this,0,0,b,c);

	for(var d=0;d<c;d++){
		this[d]=%_Arguments(d);
	}

	this.length=b+c;

	return b+c;
}


function ArraySlice(a,b){
	var c=(this.length>>>0);
	var d=(%_IsSmi(%IS_VAR(a))?a:%NumberToInteger(ToNumber(a)));
	var f=c;

	if(b!==void 0)f=(%_IsSmi(%IS_VAR(b))?b:%NumberToInteger(ToNumber(b)));

	if(d<0){
		d+=c;
		if(d<0)d=0;
	}else{
		if(d>c)d=c;
	}

	if(f<0){
		f+=c;
		if(f<0)f=0;
	}else{
		if(f>c)f=c;
	}

	var g=[];

	if(f<d)return g;

	if((%_IsArray(this))){
		SmartSlice(this,d,f-d,c,g);
	}else{
		SimpleSlice(this,d,f-d,c,g);
	}

	g.length=f-d;

	return g;
}


function ArraySplice(a,b){
	var c=%_ArgumentsLength();

	var d=(this.length>>>0);
	var f=(%_IsSmi(%IS_VAR(a))?a:%NumberToInteger(ToNumber(a)));

	if(f<0){
		f+=d;
		if(f<0)f=0;
	}else{
		if(f>d)f=d;
	}





	var g=0;
	if(c>1){
		g=(%_IsSmi(%IS_VAR(b))?b:%NumberToInteger(ToNumber(b)));
		if(g<0)g=0;
		if(g>d-f)g=d-f;
	}else{
		g=d-f;
	}

	var h=[];
	h.length=g;


	var i=0;
	if(c>2){
		i=c-2;
	}

	var j=true;

	if((%_IsArray(this))&&i!==g){



		var k=%EstimateNumberOfElements(this);
		if(d>20&&(k>>2)<(d-f)){
			j=false;
		}
	}

	if(j){
		SimpleSlice(this,f,g,d,h);
		SimpleMove(this,f,g,d,i);
	}else{
		SmartSlice(this,f,g,d,h);
		SmartMove(this,f,g,d,i);
	}



	var l=f;
	var m=2;
	var n=%_ArgumentsLength();
	while(m<n){
		this[l++]=%_Arguments(m++);
	}
	this.length=d-g+i;


	return h;
}


function ArraySort(a){



	if(!(%_IsFunction(a))){
		a=function(b,c){
			if(b===c)return 0;
			if(%_IsSmi(b)&&%_IsSmi(c)){
				return %SmiLexicographicCompare(b,c);
			}
			b=ToString(b);
			c=ToString(c);
			if(b==c)return 0;
			else return b<c?-1:1;
		};
	}
	var d=%GetGlobalReceiver();

	function InsertionSort(f,g,h){
		for(var i=g+1;i<h;i++){
			var j=f[i];
			for(var k=i-1;k>=g;k--){
				var l=f[k];
				var m=%_CallFunction(d,l,j,a);
				if(m>0){
					f[k+1]=l;
				}else{
					break;
				}
			}
			f[k+1]=j;
		}
	}

	function QuickSort(f,g,h){

		if(h-g<=22){
			InsertionSort(f,g,h);
			return;
		}
		var n=$floor($random()*(h-g))+g;
		var o=f[n];


		%_SwapElements(f,g,n);
		var q=g;
		var r=h;


		for(var i=g+1;i<r;){
			var j=f[i];
			var m=%_CallFunction(d,j,o,a);
			if(m<0){
				%_SwapElements(f,i,q);
				i++;
				q++;
			}else if(m>0){
				r--;
				%_SwapElements(f,i,r);
			}else{
				i++;
			}
		}
		QuickSort(f,g,q);
		QuickSort(f,r,h);
	}




	function CopyFromPrototype(s,t){
		var u=0;
		for(var v=s.__proto__;v;v=v.__proto__){
			var w=%GetArrayKeys(v,t);
			if(w.length>0){
				if(w[0]==-1){

					var z=w[1];
					for(var i=0;i<z;i++){
						if(!s.hasOwnProperty(i)&&v.hasOwnProperty(i)){
							s[i]=v[i];
							if(i>=u){u=i+1;}
						}
					}
				}else{
					for(var i=0;i<w.length;i++){
						var A=w[i];
						if(!(typeof(A)==='undefined')&&
								!s.hasOwnProperty(A)&&v.hasOwnProperty(A)){
									s[A]=v[A];
									if(A>=u){u=A+1;}
								}
					}
				}
			}
		}
		return u;
	}




	function ShadowPrototypeElements(s,g,h){
		for(var v=s.__proto__;v;v=v.__proto__){
			var w=%GetArrayKeys(v,h);
			if(w.length>0){
				if(w[0]==-1){

					var z=w[1];
					for(var i=g;i<z;i++){
						if(v.hasOwnProperty(i)){
							s[i]=void 0;
						}
					}
				}else{
					for(var i=0;i<w.length;i++){
						var A=w[i];
						if(!(typeof(A)==='undefined')&&g<=A&&
								v.hasOwnProperty(A)){
									s[A]=void 0;
								}
					}
				}
			}
		}
	}

	function SafeRemoveArrayHoles(s){



		var B=0;
		var C=t-1;
		var D=0;
		while(B<C){

			while(B<C&&
					!(typeof(s[B])==='undefined')){
						B++;
					}


			if(!s.hasOwnProperty(B)){
				D++;
			}


			while(B<C&&
					(typeof(s[C])==='undefined')){
						if(!s.hasOwnProperty(C)){
							D++;
						}
						C--;
					}
			if(B<C){

				s[B]=s[C];
				s[C]=void 0;
			}
		}




		if(!(typeof(s[B])==='undefined'))B++;


		var i;
		for(i=B;i<t-D;i++){
			s[i]=void 0;
		}
		for(i=t-D;i<t;i++){

			if(i in s.__proto__){
				s[i]=void 0;
			}else{
				delete s[i];
			}
		}


		return B;
	}

	var t=(this.length>>>0);
	if(t<2)return this;

	var E=(%_IsArray(this));
	var G;
	if(!E){








		G=CopyFromPrototype(this,t);
	}

	var H=%RemoveArrayHoles(this,t);
	if(H==-1){



		H=SafeRemoveArrayHoles(this);
	}

	QuickSort(this,0,H);

	if(!E&&(H+1<G)){


		ShadowPrototypeElements(this,H,G);
	}

	return this;
}





function ArrayFilter(a,b){
	if(!(%_IsFunction(a))){
		throw MakeTypeError('called_non_callable',[a]);
	}


	var c=this.length;
	var d=[];
	var g=0;
	for(var h=0;h<c;h++){
		var i=this[h];
		if(!(typeof(i)==='undefined')||h in this){
			if(a.call(b,i,h,this))d[g++]=i;
		}
	}
	return d;
}


function ArrayForEach(a,b){
	if(!(%_IsFunction(a))){
		throw MakeTypeError('called_non_callable',[a]);
	}


	var c=(this.length>>>0);
	for(var d=0;d<c;d++){
		var g=this[d];
		if(!(typeof(g)==='undefined')||d in this){
			a.call(b,g,d,this);
		}
	}
}




function ArraySome(a,b){
	if(!(%_IsFunction(a))){
		throw MakeTypeError('called_non_callable',[a]);
	}


	var c=(this.length>>>0);
	for(var d=0;d<c;d++){
		var g=this[d];
		if(!(typeof(g)==='undefined')||d in this){
			if(a.call(b,g,d,this))return true;
		}
	}
	return false;
}


function ArrayEvery(a,b){
	if(!(%_IsFunction(a))){
		throw MakeTypeError('called_non_callable',[a]);
	}


	var c=(this.length>>>0);
	for(var d=0;d<c;d++){
		var g=this[d];
		if(!(typeof(g)==='undefined')||d in this){
			if(!a.call(b,g,d,this))return false;
		}
	}
	return true;
}

function ArrayMap(a,b){
	if(!(%_IsFunction(a))){
		throw MakeTypeError('called_non_callable',[a]);
	}


	var c=(this.length>>>0);
	var d=new $Array(c);
	for(var g=0;g<c;g++){
		var h=this[g];
		if(!(typeof(h)==='undefined')||g in this){
			d[g]=a.call(b,h,g,this);
		}
	}
	return d;
}


function ArrayIndexOf(a,b){
	var c=(this.length>>>0);
	if(c==0)return-1;
	if((typeof(b)==='undefined')){
		b=0;
	}else{
		b=(%_IsSmi(%IS_VAR(b))?b:%NumberToInteger(ToNumber(b)));

		if(b<0)b=c+b;

		if(b<0)b=0;
	}
	var d=b;
	var g=c;
	if(UseSparseVariant(this,c,true)){
		var h=%GetArrayKeys(this,c);
		if(h.length==2&&h[0]<0){

			var i=-(h[0]+1);
			var j=i+h[1];
			d=MAX(d,i);
			g=j;

		}else{
			if(h.length==0)return-1;

			var k=GetSortedArrayKeys(this,h);
			var l=k.length;
			var m=0;
			while(m<l&&k[m]<b)m++;
			while(m<l){
				var n=k[m];
				if(!(typeof(n)==='undefined')&&this[n]===a)return n;
				m++;
			}
			return-1;
		}
	}

	if(!(typeof(a)==='undefined')){
		for(var m=d;m<g;m++){
			if(this[m]===a)return m;
		}
		return-1;
	}

	for(var m=d;m<g;m++){
		if((typeof(this[m])==='undefined')&&m in this){
			return m;
		}
	}
	return-1;
}


function ArrayLastIndexOf(a,b){
	var c=(this.length>>>0);
	if(c==0)return-1;
	if(%_ArgumentsLength()<2){
		b=c-1;
	}else{
		b=(%_IsSmi(%IS_VAR(b))?b:%NumberToInteger(ToNumber(b)));

		if(b<0)b+=c;

		if(b<0)return-1;
		else if(b>=c)b=c-1;
	}
	var d=0;
	var g=b;
	if(UseSparseVariant(this,c,true)){
		var h=%GetArrayKeys(this,b+1);
		if(h.length==2&&h[0]<0){

			var i=-(h[0]+1);
			var j=i+h[1];
			d=MAX(d,i);
			g=j;

		}else{
			if(h.length==0)return-1;

			var k=GetSortedArrayKeys(this,h);
			var l=k.length-1;
			while(l>=0){
				var m=k[l];
				if(!(typeof(m)==='undefined')&&this[m]===a)return m;
				l--;
			}
			return-1;
		}
	}

	if(!(typeof(a)==='undefined')){
		for(var l=g;l>=d;l--){
			if(this[l]===a)return l;
		}
		return-1;
	}
	for(var l=g;l>=d;l--){
		if((typeof(this[l])==='undefined')&&l in this){
			return l;
		}
	}
	return-1;
}


function ArrayReduce(a,b){
	if(!(%_IsFunction(a))){
		throw MakeTypeError('called_non_callable',[a]);
	}


	var c=this.length;
	var d=0;

	find_initial:if(%_ArgumentsLength()<2){
		for(;d<c;d++){
			b=this[d];
			if(!(typeof(b)==='undefined')||d in this){
				d++;
				break find_initial;
			}
		}
		throw MakeTypeError('reduce_no_initial',[]);
	}

				 for(;d<c;d++){
					 var g=this[d];
					 if(!(typeof(g)==='undefined')||d in this){
						 b=a.call(null,b,g,d,this);
					 }
				 }
				 return b;
}

function ArrayReduceRight(a,b){
	if(!(%_IsFunction(a))){
		throw MakeTypeError('called_non_callable',[a]);
	}
	var c=this.length-1;

	find_initial:if(%_ArgumentsLength()<2){
		for(;c>=0;c--){
			b=this[c];
			if(!(typeof(b)==='undefined')||c in this){
				c--;
				break find_initial;
			}
		}
		throw MakeTypeError('reduce_no_initial',[]);
	}

				 for(;c>=0;c--){
					 var d=this[c];
					 if(!(typeof(d)==='undefined')||c in this){
						 b=a.call(null,b,d,c,this);
					 }
				 }
				 return b;
}


function ArrayIsArray(a){
	return(%_IsArray(a));
}



function SetupArray(){


	%SetProperty($Array.prototype,"constructor",$Array,2);


	InstallFunctions($Array,2,$Array(
				"isArray",ArrayIsArray
				));

	var a=%SpecialArrayFunctions({});

	function getFunction(b,c,d){
		var g=c;
		if(a.hasOwnProperty(b)){
			g=a[b];
		}
		if(!(typeof(d)==='undefined')){
			%FunctionSetLength(g,d);
		}
		return g;
	}





	InstallFunctionsOnHiddenPrototype($Array.prototype,2,$Array(
				"toString",getFunction("toString",ArrayToString),
				"toLocaleString",getFunction("toLocaleString",ArrayToLocaleString),
				"join",getFunction("join",ArrayJoin),
				"pop",getFunction("pop",ArrayPop),
				"push",getFunction("push",ArrayPush,1),
				"concat",getFunction("concat",ArrayConcat,1),
				"reverse",getFunction("reverse",ArrayReverse),
				"shift",getFunction("shift",ArrayShift),
				"unshift",getFunction("unshift",ArrayUnshift,1),
				"slice",getFunction("slice",ArraySlice,2),
				"splice",getFunction("splice",ArraySplice,2),
				"sort",getFunction("sort",ArraySort),
				"filter",getFunction("filter",ArrayFilter,1),
				"forEach",getFunction("forEach",ArrayForEach,1),
				"some",getFunction("some",ArraySome,1),
				"every",getFunction("every",ArrayEvery,1),
				"map",getFunction("map",ArrayMap,1),
				"indexOf",getFunction("indexOf",ArrayIndexOf,1),
				"lastIndexOf",getFunction("lastIndexOf",ArrayLastIndexOf,1),
				"reduce",getFunction("reduce",ArrayReduce,1),
				"reduceRight",getFunction("reduceRight",ArrayReduceRight,1)
					));

	%FinishArrayPrototypeSetup($Array.prototype);
}


SetupArray();




































%SetCode($String,function(a){
	var b=%_ArgumentsLength()==0?'':((typeof(%IS_VAR(a))==='string')?a:NonStringToString(a));
	if(%_IsConstructCall()){
		%_SetValueOf(this,b);
	}else{
		return b;
	}
});

%FunctionSetPrototype($String,new $String());


function StringToString(){
	if(!(typeof(this)==='string')&&!(%_ClassOf(this)==='String'))
		throw new $TypeError('String.prototype.toString is not generic');
	return %_ValueOf(this);
}



function StringValueOf(){
	if(!(typeof(this)==='string')&&!(%_ClassOf(this)==='String'))
		throw new $TypeError('String.prototype.valueOf is not generic');
	return %_ValueOf(this);
}



function StringCharAt(a){
	var b=%_StringCharAt(this,a);
	if(%_IsSmi(b)){
		b=%_StringCharAt(((typeof(%IS_VAR(this))==='string')?this:NonStringToString(this)),(%_IsSmi(%IS_VAR(a))?a:%NumberToInteger(ToNumber(a))));
	}
	return b;
}



function StringCharCodeAt(a){
	var b=%_StringCharCodeAt(this,a);
	if(!%_IsSmi(b)){
		b=%_StringCharCodeAt(((typeof(%IS_VAR(this))==='string')?this:NonStringToString(this)),(%_IsSmi(%IS_VAR(a))?a:%NumberToInteger(ToNumber(a))));
	}
	return b;
}



function StringConcat(){
	var a=%_ArgumentsLength();
	var b=((typeof(%IS_VAR(this))==='string')?this:NonStringToString(this));
	if(a===1){
		return b+%_Arguments(0);
	}
	var c=new $Array(a+1);
	c[0]=b;
	for(var d=0;d<a;d++){
		var g=%_Arguments(d);
		c[d+1]=((typeof(%IS_VAR(g))==='string')?g:NonStringToString(g));
	}
	return %StringBuilderConcat(c,a+1,"");
}


%FunctionSetLength(StringConcat,1);



function StringIndexOf(a){
	var b=((typeof(%IS_VAR(this))==='string')?this:NonStringToString(this));
	var c=((typeof(%IS_VAR(a))==='string')?a:NonStringToString(a));
	var d=b.length;
	var g=c.length;
	var h=0;
	if(%_ArgumentsLength()>1){
		var i=%_Arguments(1);
		h=(%_IsSmi(%IS_VAR(i))?i:%NumberToInteger(ToNumber(i)));
	}
	if(h<0)h=0;
	if(h>d)h=d;
	if(g+h>d)return-1;
	return %StringIndexOf(b,c,h);
}



function StringLastIndexOf(a){
	var b=((typeof(%IS_VAR(this))==='string')?this:NonStringToString(this));
	var c=b.length;
	var d=((typeof(%IS_VAR(a))==='string')?a:NonStringToString(a));
	var g=d.length;
	var h=c-g;
	if(%_ArgumentsLength()>1){
		var i=ToNumber(%_Arguments(1));
		if(!$isNaN(i)){
			i=(%_IsSmi(%IS_VAR(i))?i:%NumberToInteger(ToNumber(i)));
			if(i<0){
				i=0;
			}
			if(i+g<c){
				h=i
			}
		}
	}
	if(h<0){
		return-1;
	}
	return %StringLastIndexOf(b,d,h);
}


function CloneDenseArray(a){
	if(a===null)return null;
	var b=new $Array(a.length);
	for(var c=0;c<a.length;c++){
		b[c]=a[c];
	}
	return b;
}






function StringLocaleCompare(a){
	if(%_ArgumentsLength()===0)return 0;

	var b=((typeof(%IS_VAR(this))==='string')?this:NonStringToString(this));
	var c=((typeof(%IS_VAR(a))==='string')?a:NonStringToString(a));
	return %StringLocaleCompare(b,c);
}



function StringMatch(a){
	var b=((typeof(%IS_VAR(this))==='string')?this:NonStringToString(this));
	if((%_IsRegExp(a))){
		if(!a.global)return a.exec(b);

		var c=regExpCache;
		var d=false;

		if(%_ObjectEquals(c.type,'match')&&
				%_IsRegExpEquivalent(c.regExp,a)&&
				%_ObjectEquals(c.subject,b)){
					if(c.answerSaved){
						return CloneDenseArray(c.answer);
					}else{
						d=true;
					}
				}
		%_Log('regexp','regexp-match,%0S,%1r',[b,a]);

		var g=%StringMatch(b,a,lastMatchInfo);
		c.type='match';
		c.regExp=a;
		c.subject=b;
		if(d)c.answer=CloneDenseArray(g);
		c.answerSaved=d;
		return g;
	}

	a=new $RegExp(a);


	return RegExpExecNoTests(a,b,0);
}





function SubString(a,b,c){

	if(b+1==c){
		return %_StringCharAt(a,b);
	}
	return %_SubString(a,b,c);
}







var reusableMatchInfo=[2,"","",-1,-1];



function StringReplace(a,b){
	var c=((typeof(%IS_VAR(this))==='string')?this:NonStringToString(this));


	if((%_IsRegExp(a))){
		%_Log('regexp','regexp-replace,%0r,%1S',[a,c]);
		if((%_IsFunction(b))){
			regExpCache.type='none';
			if(a.global){
				return StringReplaceGlobalRegExpWithFunction(c,a,b);
			}else{
				return StringReplaceNonGlobalRegExpWithFunction(c,
						a,
						b);
			}
		}else{
			return StringReplaceRegExp(c,a,b);
		}
	}


	a=((typeof(%IS_VAR(a))==='string')?a:NonStringToString(a));
	var d=%StringIndexOf(c,a,0);
	if(d<0)return c;
	var g=d+a.length;

	var h=new ReplaceResultBuilder(c);

	h.addSpecialSlice(0,d);


	if((%_IsFunction(b))){
		h.add(b.call(null,a,d,c));
	}else{
		reusableMatchInfo[3]=d;
		reusableMatchInfo[4]=g;
		b=((typeof(%IS_VAR(b))==='string')?b:NonStringToString(b));
		ExpandReplacement(b,c,reusableMatchInfo,h);
	}


	h.addSpecialSlice(g,c.length);

	return h.generate();
}



function StringReplaceRegExp(a,b,c){
	var d=regExpCache;
	if(%_ObjectEquals(d.type,'replace')&&
			%_IsRegExpEquivalent(d.regExp,b)&&
			%_ObjectEquals(d.replaceString,c)&&
			%_ObjectEquals(d.subject,a)){
				return d.answer;
			}
	c=((typeof(%IS_VAR(c))==='string')?c:NonStringToString(c));
	var g=%StringReplaceRegExpWithString(a,
			b,
			c,
			lastMatchInfo);
	d.subject=a;
	d.regExp=b;
	d.replaceString=c;
	d.answer=g;
	d.type='replace';
	return g;
}




function ExpandReplacement(a,b,c,d){
	var g=%StringIndexOf(a,'$',0);
	if(g<0){
		d.add(a);
		return;
	}


	var h=((c)[0])>>1;

	if(g>0)d.add(SubString(a,0,g));
	var i=a.length;

	while(true){
		var j='$';
		var k=g+1;
		if(k<i){
			var l=%_StringCharCodeAt(a,k);
			if(l==36){
				++k;
				d.add('$');
			}else if(l==38){
				++k;
				d.addSpecialSlice(c[3],
						c[4]);
			}else if(l==96){
				++k;
				d.addSpecialSlice(0,c[3]);
			}else if(l==39){
				++k;
				d.addSpecialSlice(c[4],b.length);
			}else if(l>=48&&l<=57){
				++k;
				var m=l-48;
				if(k<i){
					l=%_StringCharCodeAt(a,k);

					if(m!=0&&l==48||l>=49&&l<=57){
						var n=m*10+(l-48);
						if(n<h){





							++k;
							m=n;
						}
					}
				}
				if(0<m&&m<h){
					addCaptureString(d,c,m);
				}else{




					d.add('$');
					--k;
				}
			}else{
				d.add('$');
			}
		}else{
			d.add('$');
		}


		g=%StringIndexOf(a,'$',k);



		if(g<0){
			if(k<i){
				d.add(SubString(a,k,i));
			}
			return;
		}


		d.add(SubString(a,k,g));
	}
};



function CaptureString(a,b,c){

	var d=c<<1;

	var g=b[(3+(d))];

	if(g<0)return;
	var h=b[(3+(d+1))];
	return SubString(a,g,h);
};




function addCaptureString(a,b,c){

	var d=c<<1;

	var g=b[(3+(d))];
	if(g<0)return;
	var h=b[(3+(d+1))];
	a.addSpecialSlice(g,h);
};




var reusableReplaceArray=$Array(16);



function StringReplaceGlobalRegExpWithFunction(a,b,c){
	var d=reusableReplaceArray;
	if(d){
		reusableReplaceArray=null;
	}else{




		d=$Array(16);
	}
	var g=%RegExpExecMultiple(b,
			a,
			lastMatchInfo,
			d);
	b.lastIndex=0;
	if((g===null)){

		reusableReplaceArray=d;
		return a;
	}
	var h=g.length;
	var i=0;
	if(((lastMatchInfo)[0])==2){
		var j=0;
		var k=[null,0,a];
		var l=%GetGlobalReceiver();
		while(i<h){
			var m=g[i];
			if(%_IsSmi(m)){
				if(m>0){
					j=(m>>11)+(m&0x7ff);
				}else{
					j=g[++i]-m;
				}
			}else{
				k[0]=m;
				k[1]=j;
				lastMatchInfoOverride=k;
				var n=
					%_CallFunction(l,m,j,a,c);
				if(!(typeof(n)==='string')){
					n=NonStringToString(n);
				}
				g[i]=n;
				j+=m.length;
			}
			i++;
		}
	}else{
		while(i<h){
			var m=g[i];
			if(!%_IsSmi(m)){


				lastMatchInfoOverride=m;
				var n=c.apply(null,m);
				if(!(typeof(n)==='string')){
					n=NonStringToString(n);
				}
				g[i]=n;
			}
			i++;
		}
	}
	var o=new ReplaceResultBuilder(a,g);
	var q=o.generate();
	d.length=0;
	reusableReplaceArray=d;
	return q;
}


function StringReplaceNonGlobalRegExpWithFunction(a,b,c){
	var d=DoRegExpExec(b,a,0);
	if((d===null))return a;
	var g=new ReplaceResultBuilder(a);
	var h=d[3];
	g.addSpecialSlice(0,h);
	var i=d[4];



	var j=((d)[0])>>1;
	var k;
	if(j==1){

		var l=SubString(a,h,i);

		k=
			%_CallFunction(%GetGlobalReceiver(),l,h,a,c);
	}else{
		var m=$Array(j+2);
		for(var n=0;n<j;n++){
			m[n]=CaptureString(a,d,n);
		}
		m[n]=h;
		m[n+1]=a;

		k=c.apply(null,m);
	}

	g.add(k);


	g.addSpecialSlice(i,a.length);
	return g.generate();
}



function StringSearch(a){
	var b;
	if((typeof(a)==='string')){
		b=%_GetFromCache(0,a);
	}else if((%_IsRegExp(a))){
		b=a;
	}else{
		b=new $RegExp(a);
	}
	var c=((typeof(%IS_VAR(this))==='string')?this:NonStringToString(this));
	var d=DoRegExpExec(b,c,0);
	if(d){
		return d[3];
	}
	return-1;
}



function StringSlice(a,b){
	var c=((typeof(%IS_VAR(this))==='string')?this:NonStringToString(this));
	var d=c.length;
	var g=(%_IsSmi(%IS_VAR(a))?a:%NumberToInteger(ToNumber(a)));
	var h=d;
	if(b!==void 0)
		h=(%_IsSmi(%IS_VAR(b))?b:%NumberToInteger(ToNumber(b)));

	if(g<0){
		g+=d;
		if(g<0)
			g=0;
	}else{
		if(g>d)
			g=d;
	}

	if(h<0){
		h+=d;
		if(h<0)
			h=0;
	}else{
		if(h>d)
			h=d;
	}

	var i=h-g;
	if(i<0)
		i=0;

	return SubString(c,g,g+i);
}



function StringSplit(a,b){
	var c=((typeof(%IS_VAR(this))==='string')?this:NonStringToString(this));
	b=((typeof(b)==='undefined'))?0xffffffff:(b>>>0);
	if(b===0)return[];






	if(%_ArgumentsLength()===0){
		return[c];
	}

	var d=c.length;
	if(!(%_IsRegExp(a))){
		a=((typeof(%IS_VAR(a))==='string')?a:NonStringToString(a));
		var g=a.length;


		if(g===0)return %StringToArray(c);

		var h=%StringSplit(c,a,b);

		return h;
	}

	var i=regExpCache;
	var j=false;

	if(%_ObjectEquals(i.type,'split')&&
			%_IsRegExpEquivalent(i.regExp,a)&&
			%_ObjectEquals(i.subject,c)&&
			%_ObjectEquals(i.lastIndex,b)){
				if(i.answerSaved){
					return CloneDenseArray(i.answer);
				}else{
					j=true;
				}
			}

	i.type='split';
	i.regExp=a;
	i.subject=c;

	i.lastIndex=b;

	%_Log('regexp','regexp-split,%0S,%1r',[c,a]);

	if(d===0){
		i.answerSaved=true;
		if(splitMatch(a,c,0,0)!=null){
			i.answer=[];
			return[];
		}
		i.answer=[c];
		return[c];
	}

	var k=0;
	var l=0;
	var h=[];

	outer_loop:
		while(true){

			if(l===d){
				h[h.length]=c.slice(k,d);
				break;
			}

			var m=splitMatch(a,c,k,l);

			if((m===null)){
				h[h.length]=c.slice(k,d);
				break;
			}

			var n=m[4];


			if(l===n&&n===k){
				l++;
				continue;
			}

			h[h.length]=SubString(c,k,m[3]);
			if(h.length===b)break;

			var o=((m)[0]);
			for(var q=2;q<o;q+=2){
				var r=m[(3+(q))];
				var s=m[(3+(q+1))];
				if(r!=-1&&s!=-1){
					h[h.length]=SubString(c,r,s);
				}else{
					h[h.length]=void 0;
				}
				if(h.length===b)break outer_loop;
			}

			l=k=n;
		}
	if(j)i.answer=CloneDenseArray(h);
	i.answerSaved=j;
	return h;
}





function splitMatch(a,b,c,d){
	var g=DoRegExpExec(a,b,d);
	if(g==null)return null;


	if(g[3]===b.length)return null;
	return g;
}



function StringSubstring(a,b){
	var c=((typeof(%IS_VAR(this))==='string')?this:NonStringToString(this));
	var d=c.length;

	var g=(%_IsSmi(%IS_VAR(a))?a:%NumberToInteger(ToNumber(a)));
	if(g<0){
		g=0;
	}else if(g>d){
		g=d;
	}

	var h=d;
	if(!(typeof(b)==='undefined')){
		h=(%_IsSmi(%IS_VAR(b))?b:%NumberToInteger(ToNumber(b)));
		if(h>d){
			h=d;
		}else{
			if(h<0)h=0;
			if(g>h){
				var i=h;
				h=g;
				g=i;
			}
		}
	}

	return SubString(c,g,h);
}



function StringSubstr(a,b){
	var c=((typeof(%IS_VAR(this))==='string')?this:NonStringToString(this));
	var d;



	if(b===void 0){
		d=c.length;
	}else{
		d=(%_IsSmi(%IS_VAR(b))?b:%NumberToInteger(ToNumber(b)));
		if(d<=0)return'';
	}



	if(a===void 0){
		a=0;
	}else{
		a=(%_IsSmi(%IS_VAR(a))?a:%NumberToInteger(ToNumber(a)));


		if(a>=c.length)return'';


		if(a<0){
			a+=c.length;
			if(a<0)a=0;
		}
	}

	var g=a+d;
	if(g>c.length)g=c.length;

	return SubString(c,a,g);
}



function StringToLowerCase(){
	return %StringToLowerCase(((typeof(%IS_VAR(this))==='string')?this:NonStringToString(this)));
}



function StringToLocaleLowerCase(){
	return %StringToLowerCase(((typeof(%IS_VAR(this))==='string')?this:NonStringToString(this)));
}



function StringToUpperCase(){
	return %StringToUpperCase(((typeof(%IS_VAR(this))==='string')?this:NonStringToString(this)));
}



function StringToLocaleUpperCase(){
	return %StringToUpperCase(((typeof(%IS_VAR(this))==='string')?this:NonStringToString(this)));
}


function StringTrim(){
	return %StringTrim(((typeof(%IS_VAR(this))==='string')?this:NonStringToString(this)),true,true);
}

function StringTrimLeft(){
	return %StringTrim(((typeof(%IS_VAR(this))==='string')?this:NonStringToString(this)),true,false);
}

function StringTrimRight(){
	return %StringTrim(((typeof(%IS_VAR(this))==='string')?this:NonStringToString(this)),false,true);
}

var static_charcode_array=new $Array(4);


function StringFromCharCode(a){
	var b=%_ArgumentsLength();
	if(b==1){
		if(!%_IsSmi(a))a=ToNumber(a);
		return %_StringCharFromCode(a&0xffff);
	}




	var c=static_charcode_array;
	for(var d=0;d<b;d++){
		var a=%_Arguments(d);
		if(!%_IsSmi(a))a=ToNumber(a);
		c[d]=a;
	}
	c.length=b;
	return %StringFromCharCodeArray(c);
}



function HtmlEscape(a){
	return((typeof(%IS_VAR(a))==='string')?a:NonStringToString(a)).replace(/</g,"&lt;")
		.replace(/>/g,"&gt;")
		.replace(/"/g,"&quot;")
		.replace(/'/g,"&#039;");
};




function StringLink(a){
	return"<a href=\""+HtmlEscape(a)+"\">"+this+"</a>";
}


function StringAnchor(a){
	return"<a name=\""+HtmlEscape(a)+"\">"+this+"</a>";
}


function StringFontcolor(a){
	return"<font color=\""+HtmlEscape(a)+"\">"+this+"</font>";
}


function StringFontsize(a){
	return"<font size=\""+HtmlEscape(a)+"\">"+this+"</font>";
}


function StringBig(){
	return"<big>"+this+"</big>";
}


function StringBlink(){
	return"<blink>"+this+"</blink>";
}


function StringBold(){
	return"<b>"+this+"</b>";
}


function StringFixed(){
	return"<tt>"+this+"</tt>";
}


function StringItalics(){
	return"<i>"+this+"</i>";
}


function StringSmall(){
	return"<small>"+this+"</small>";
}


function StringStrike(){
	return"<strike>"+this+"</strike>";
}


function StringSub(){
	return"<sub>"+this+"</sub>";
}


function StringSup(){
	return"<sup>"+this+"</sup>";
}



function ReplaceResultBuilder(a){
	if(%_ArgumentsLength()>1){
		this.elements=%_Arguments(1);
	}else{
		this.elements=new $Array();
	}
	this.special_string=a;
}


ReplaceResultBuilder.prototype.add=function(a){
	a=((typeof(%IS_VAR(a))==='string')?a:NonStringToString(a));
	if(a.length>0){
		var b=this.elements;
		b[b.length]=a;
	}
}


ReplaceResultBuilder.prototype.addSpecialSlice=function(a,b){
	var c=b-a;
	if(a<0||c<=0)return;
	var d=this.elements;
	if(a<0x80000&&c<0x800){
		d[d.length]=(a<<11)|c;
	}else{


		d[d.length]=-c;
		d[d.length]=a;
	}
}


ReplaceResultBuilder.prototype.generate=function(){
	var a=this.elements;
	return %StringBuilderConcat(a,a.length,this.special_string);
}


function StringToJSON(a){
	return CheckJSONPrimitive(this.valueOf());
}




function SetupString(){

	%SetProperty($String.prototype,"constructor",$String,2);



	InstallFunctions($String,2,$Array(
				"fromCharCode",StringFromCharCode
				));



	InstallFunctionsOnHiddenPrototype($String.prototype,2,$Array(
				"valueOf",StringValueOf,
				"toString",StringToString,
				"charAt",StringCharAt,
				"charCodeAt",StringCharCodeAt,
				"concat",StringConcat,
				"indexOf",StringIndexOf,
				"lastIndexOf",StringLastIndexOf,
				"localeCompare",StringLocaleCompare,
				"match",StringMatch,
				"replace",StringReplace,
				"search",StringSearch,
				"slice",StringSlice,
				"split",StringSplit,
				"substring",StringSubstring,
				"substr",StringSubstr,
				"toLowerCase",StringToLowerCase,
				"toLocaleLowerCase",StringToLocaleLowerCase,
				"toUpperCase",StringToUpperCase,
				"toLocaleUpperCase",StringToLocaleUpperCase,
				"trim",StringTrim,
				"trimLeft",StringTrimLeft,
				"trimRight",StringTrimRight,
				"link",StringLink,
				"anchor",StringAnchor,
				"fontcolor",StringFontcolor,
				"fontsize",StringFontsize,
				"big",StringBig,
				"blink",StringBlink,
				"bold",StringBold,
				"fixed",StringFixed,
				"italics",StringItalics,
				"small",StringSmall,
				"strike",StringStrike,
				"sub",StringSub,
				"sup",StringSup,
				"toJSON",StringToJSON
					));
}


SetupString();


































var hexCharArray=0;
var hexCharCodeArray=0;


function URIAddEncodedOctetToBuffer(a,b,c){
	b[c++]=37;
	b[c++]=hexCharCodeArray[a>>4];
	b[c++]=hexCharCodeArray[a&0x0F];
	return c;
}


function URIEncodeOctets(a,b,c){
	if(hexCharCodeArray===0){
		hexCharCodeArray=[48,49,50,51,52,53,54,55,56,57,
			65,66,67,68,69,70];
	}
	c=URIAddEncodedOctetToBuffer(a[0],b,c);
	if(a[1])c=URIAddEncodedOctetToBuffer(a[1],b,c);
	if(a[2])c=URIAddEncodedOctetToBuffer(a[2],b,c);
	if(a[3])c=URIAddEncodedOctetToBuffer(a[3],b,c);
	return c;
}


function URIEncodeSingle(a,b,c){
	var d=(a>>12)&0xF;
	var g=(a>>6)&63;
	var h=a&63;
	var i=new $Array(3);
	if(a<=0x007F){
		i[0]=a;
	}else if(a<=0x07FF){
		i[0]=g+192;
		i[1]=h+128;
	}else{
		i[0]=d+224;
		i[1]=g+128;
		i[2]=h+128;
	}
	return URIEncodeOctets(i,b,c);
}


function URIEncodePair(a,b,c,d){
	var g=((a>>6)&0xF)+1;
	var h=(a>>2)&0xF;
	var i=a&3;
	var j=(b>>6)&0xF;
	var k=b&63;
	var l=new $Array(4);
	l[0]=(g>>2)+240;
	l[1]=(((g&3)<<4)|h)+128;
	l[2]=((i<<4)|j)+128;
	l[3]=k+128;
	return URIEncodeOctets(l,c,d);
}


function URIHexCharsToCharCode(a,b){
	if(HexValueOf(a)==-1||HexValueOf(b)==-1){
		throw new $URIError("URI malformed");
	}
	return HexStrToCharCode(a+b);
}


function URIDecodeOctets(a,b,c){
	var d;
	var g=a[0];
	if(g<0x80){
		d=g;
	}else if(g<0xc2){
		throw new $URIError("URI malformed");
	}else{
		var h=a[1];
		if(g<0xe0){
			var i=g&0x1f;
			if((h<0x80)||(h>0xbf))
				throw new $URIError("URI malformed");
			var j=h&0x3f;
			d=(i<<6)+j;
			if(d<0x80||d>0x7ff)
				throw new $URIError("URI malformed");
		}else{
			var k=a[2];
			if(g<0xf0){
				var i=g&0x0f;
				if((h<0x80)||(h>0xbf))
					throw new $URIError("URI malformed");
				var j=h&0x3f;
				if((k<0x80)||(k>0xbf))
					throw new $URIError("URI malformed");
				var l=k&0x3f;
				d=(i<<12)+(j<<6)+l;
				if((d<0x800)||(d>0xffff))
					throw new $URIError("URI malformed");
			}else{
				var m=a[3];
				if(g<0xf8){
					var i=(g&0x07);
					if((h<0x80)||(h>0xbf))
						throw new $URIError("URI malformed");
					var j=(h&0x3f);
					if((k<0x80)||(k>0xbf))
						throw new $URIError("URI malformed");
					var l=(k&0x3f);
					if((m<0x80)||(m>0xbf))
						throw new $URIError("URI malformed");
					var o=(m&0x3f);
					d=(i<<18)+(j<<12)+(l<<6)+o;
					if((d<0x10000)||(d>0x10ffff))
						throw new $URIError("URI malformed");
				}else{
					throw new $URIError("URI malformed");
				}
			}
		}
	}
	if(d<0x10000){
		b[c++]=d;
		return c;
	}else{
		b[c++]=(d>>10)+0xd7c0;
		b[c++]=(d&0x3ff)+0xdc00;
		return c;
	}
}



function Encode(a,b){
	var c=a.length;
	var d=new $Array(c);
	var g=0;
	for(var h=0;h<c;h++){
		var i=a.charCodeAt(h);
		if(b(i)){
			d[g++]=i;
		}else{
			if(i>=0xDC00&&i<=0xDFFF)throw new $URIError("URI malformed");
			if(i<0xD800||i>0xDBFF){
				g=URIEncodeSingle(i,d,g);
			}else{
				h++;
				if(h==c)throw new $URIError("URI malformed");
				var j=a.charCodeAt(h);
				if(j<0xDC00||j>0xDFFF)throw new $URIError("URI malformed");
				g=URIEncodePair(i,j,d,g);
			}
		}
	}
	return %StringFromCharCodeArray(d);
}



function Decode(a,b){
	var c=a.length;
	var d=new $Array(c);
	var g=0;
	for(var h=0;h<c;h++){
		var i=a.charAt(h);
		if(i=='%'){
			if(h+2>=c)throw new $URIError("URI malformed");
			var j=URIHexCharsToCharCode(a.charAt(++h),a.charAt(++h));
			if(j>>7){
				var k=0;
				while(((j<<++k)&0x80)!=0);
				if(k==1||k>4)throw new $URIError("URI malformed");
				var l=new $Array(k);
				l[0]=j;
				if(h+3*(k-1)>=c)throw new $URIError("URI malformed");
				for(var m=1;m<k;m++){
					h++;
					l[m]=URIHexCharsToCharCode(a.charAt(++h),a.charAt(++h));
				}
				g=URIDecodeOctets(l,d,g);
			}else{
				if(b(j)){
					d[g++]=37;
					d[g++]=a.charCodeAt(h-1);
					d[g++]=a.charCodeAt(h);
				}else{
					d[g++]=j;
				}
			}
		}else{
			d[g++]=i.charCodeAt(0);
		}
	}
	d.length=g;
	return %StringFromCharCodeArray(d);
}



function URIDecode(a){
	function reservedPredicate(b){

		if(35<=b&&b<=36)return true;

		if(b==38)return true;

		if(43<=b&&b<=44)return true;

		if(b==47)return true;

		if(58<=b&&b<=59)return true;

		if(b==61)return true;

		if(63<=b&&b<=64)return true;

		return false;
	};
	var c=ToString(a);
	return Decode(c,reservedPredicate);
}



function URIDecodeComponent(a){
	function reservedPredicate(b){return false;};
	var c=ToString(a);
	return Decode(c,reservedPredicate);
}



function isAlphaNumeric(a){

	if(97<=a&&a<=122)return true;

	if(65<=a&&a<=90)return true;

	if(48<=a&&a<=57)return true;

	return false;
}



function URIEncode(a){
	function unescapePredicate(b){
		if(isAlphaNumeric(b))return true;

		if(b==33)return true;

		if(35<=b&&b<=36)return true;

		if(38<=b&&b<=47)return true;

		if(58<=b&&b<=59)return true;

		if(b==61)return true;

		if(63<=b&&b<=64)return true;

		if(b==95)return true;

		if(b==126)return true;

		return false;
	};

	var c=ToString(a);
	return Encode(c,unescapePredicate);
}



function URIEncodeComponent(a){
	function unescapePredicate(b){
		if(isAlphaNumeric(b))return true;

		if(b==33)return true;

		if(39<=b&&b<=42)return true;

		if(45<=b&&b<=46)return true;

		if(b==95)return true;

		if(b==126)return true;

		return false;
	};

	var c=ToString(a);
	return Encode(c,unescapePredicate);
}


function HexValueOf(a){
	var b=a.charCodeAt(0);


	if(b>=48&&b<=57)return b-48;

	if(b>=65&&b<=70)return b-55;

	if(b>=97&&b<=102)return b-87;

	return-1;
}




function CharCodeToHex4Str(a){
	var b="";
	if(hexCharArray===0){
		hexCharArray=["0","1","2","3","4","5","6","7","8","9",
			"A","B","C","D","E","F"];
	}
	for(var d=0;d<4;++d){
		var g=hexCharArray[a&0x0F];
		b=g+b;
		a=a>>>4;
	}
	return b;
}



function HexStrToCharCode(a){
	var b=0;
	var d=0;
	for(var g=a.length-1;g>=0;--g){
		d=d+(HexValueOf(a.charAt(g))<<b);
		b=b+4;
	}
	return d;
}



function IsValidHex(a){
	for(var b=0;b<a.length;++b){
		var d=a.charCodeAt(b);
		if((48<=d&&d<=57)||(65<=d&&d<=70)||(97<=d&&d<=102)){

		}else{
			return false;
		}
	}
	return true;
}



function URIEscape(a){
	var b=ToString(a);
	return %URIEscape(b);
}



function URIUnescape(a){
	var b=ToString(a);
	return %URIUnescape(b);
}




function SetupURI(){


	InstallFunctions(global,2,$Array(
				"escape",URIEscape,
				"unescape",URIUnescape,
				"decodeURI",URIDecode,
				"decodeURIComponent",URIDecodeComponent,
				"encodeURI",URIEncode,
				"encodeURIComponent",URIEncodeComponent
				));
}

SetupURI();

































const $floor=MathFloor;
const $random=MathRandom;
const $abs=MathAbs;



function MathConstructor(){}
%FunctionSetInstanceClassName(MathConstructor,'Math');
const $Math=new MathConstructor();
$Math.__proto__=global.Object.prototype;
%SetProperty(global,"Math",$Math,2);


function MathAbs(a){
	if(%_IsSmi(a))return a>=0?a:-a;
	if(!(typeof(a)==='number'))a=ToNumber(a);
	if(a===0)return 0;
	return a>0?a:-a;
}


function MathAcos(a){
	if(!(typeof(a)==='number'))a=ToNumber(a);
	return %Math_acos(a);
}


function MathAsin(a){
	if(!(typeof(a)==='number'))a=ToNumber(a);
	return %Math_asin(a);
}


function MathAtan(a){
	if(!(typeof(a)==='number'))a=ToNumber(a);
	return %Math_atan(a);
}




function MathAtan2(a,b){
	if(!(typeof(a)==='number'))a=ToNumber(a);
	if(!(typeof(b)==='number'))b=ToNumber(b);
	return %Math_atan2(a,b);
}


function MathCeil(a){
	if(!(typeof(a)==='number'))a=ToNumber(a);
	return %Math_ceil(a);
}


function MathCos(a){
	if(!(typeof(a)==='number'))a=ToNumber(a);
	return %_MathCos(a);
}


function MathExp(a){
	if(!(typeof(a)==='number'))a=ToNumber(a);
	return %Math_exp(a);
}


function MathFloor(a){
	if(!(typeof(a)==='number'))a=ToNumber(a);


	if(a<0x80000000&&a>0){




		return(a>>>0);
	}else{
		return %Math_floor(a);
	}
}


function MathLog(a){
	if(!(typeof(a)==='number'))a=ToNumber(a);
	return %Math_log(a);
}


function MathMax(a,b){
	var d=%_ArgumentsLength();
	if(d==0){
		return-1/0;
	}
	var g=a;
	if(!(typeof(g)==='number'))g=ToNumber(g);
	if((!%_IsSmi(%IS_VAR(g))&&!(g==g)))return g;
	for(var h=1;h<d;h++){
		var i=%_Arguments(h);
		if(!(typeof(i)==='number'))i=ToNumber(i);
		if((!%_IsSmi(%IS_VAR(i))&&!(i==i)))return i;


		if(i>g||(g===0&&i===0&&!%_IsSmi(g)&&1/g<0))g=i;
	}
	return g;
}


function MathMin(a,b){
	var d=%_ArgumentsLength();
	if(d==0){
		return 1/0;
	}
	var g=a;
	if(!(typeof(g)==='number'))g=ToNumber(g);
	if((!%_IsSmi(%IS_VAR(g))&&!(g==g)))return g;
	for(var h=1;h<d;h++){
		var i=%_Arguments(h);
		if(!(typeof(i)==='number'))i=ToNumber(i);
		if((!%_IsSmi(%IS_VAR(i))&&!(i==i)))return i;


		if(i<g||(g===0&&i===0&&!%_IsSmi(i)&&1/i<0))g=i;
	}
	return g;
}


function MathPow(a,b){
	if(!(typeof(a)==='number'))a=ToNumber(a);
	if(!(typeof(b)==='number'))b=ToNumber(b);
	return %_MathPow(a,b);
}


function MathRandom(){
	return %_RandomHeapNumber();
}


function MathRound(a){
	if(!(typeof(a)==='number'))a=ToNumber(a);
	return %RoundNumber(a);
}


function MathSin(a){
	if(!(typeof(a)==='number'))a=ToNumber(a);
	return %_MathSin(a);
}


function MathSqrt(a){
	if(!(typeof(a)==='number'))a=ToNumber(a);
	return %_MathSqrt(a);
}


function MathTan(a){
	if(!(typeof(a)==='number'))a=ToNumber(a);
	return %Math_tan(a);
}




function SetupMath(){


	%OptimizeObjectForAddingMultipleProperties($Math,8);
	%SetProperty($Math,
			"E",
			2.7182818284590452354,
			2|4|1);

	%SetProperty($Math,
			"LN10",
			2.302585092994046,
			2|4|1);

	%SetProperty($Math,
			"LN2",
			0.6931471805599453,
			2|4|1);

	%SetProperty($Math,
			"LOG2E",
			1.4426950408889634,
			2|4|1);
	%SetProperty($Math,
			"LOG10E",
			0.43429448190325176,
			2|4|1);
	%SetProperty($Math,
			"PI",
			3.1415926535897932,
			2|4|1);
	%SetProperty($Math,
			"SQRT1_2",
			0.7071067811865476,
			2|4|1);
	%SetProperty($Math,
			"SQRT2",
			1.4142135623730951,
			2|4|1);
	%ToFastProperties($Math);



	InstallFunctionsOnHiddenPrototype($Math,2,$Array(
				"random",MathRandom,
				"abs",MathAbs,
				"acos",MathAcos,
				"asin",MathAsin,
				"atan",MathAtan,
				"ceil",MathCeil,
				"cos",MathCos,
				"exp",MathExp,
				"floor",MathFloor,
				"log",MathLog,
				"round",MathRound,
				"sin",MathSin,
				"sqrt",MathSqrt,
				"tan",MathTan,
				"atan2",MathAtan2,
				"pow",MathPow,
				"max",MathMax,
				"min",MathMin
				));
};


SetupMath();
































var TYPE_NATIVE=0;
var TYPE_EXTENSION=1;
var TYPE_NORMAL=2;


var COMPILATION_TYPE_HOST=0;
var COMPILATION_TYPE_EVAL=1;
var COMPILATION_TYPE_JSON=2;


var kVowelSounds=0;
var kCapitalVowelSounds=0;


var kNoLineNumberInfo=0;




var kAddMessageAccessorsMarker={};


function GetInstanceName(a){
	if(a.length==0){
		return"";
	}
	var b=%StringToLowerCase(StringCharAt.call(a,0));
	if(kVowelSounds===0){
		kVowelSounds={a:true,e:true,i:true,o:true,u:true,y:true};
		kCapitalVowelSounds={a:true,e:true,i:true,o:true,u:true,h:true,
			f:true,l:true,m:true,n:true,r:true,s:true,x:true,y:true};
	}
	var d=kVowelSounds;
	if(a.length>1&&(StringCharAt.call(a,0)!=b)){

		var g=%StringToLowerCase(StringCharAt.call(a,1));

		if(StringCharAt.call(a,1)!=g){
			d=kCapitalVowelSounds;
		}
	}
	var h=d[b]?"an ":"a ";
	return h+a;
}


var kMessages=0;


function FormatString(a,b){
	var d=a;
	for(var g=0;g<b.length;g++){
		var h;
		try{h=ToDetailString(b[g]);}
		catch(e){h="#<error>";}
		d=ArrayJoin.call(StringSplit.call(d,"%"+g),h);
	}
	return d;
}


function ToDetailString(a){
	if(a!=null&&(%_IsObject(a))&&a.toString===$Object.prototype.toString){
		var b=a.constructor;
		if(!b)return ToString(a);
		var d=b.name;
		if(!d)return ToString(a);
		return"#<"+GetInstanceName(d)+">";
	}else{
		return ToString(a);
	}
}


function MakeGenericError(a,b,d){
	if((typeof(d)==='undefined')){
		d=[];
	}
	var g=new a(kAddMessageAccessorsMarker);
	g.type=b;
	g.arguments=d;
	return g;
}





%FunctionSetInstanceClassName(Script,'Script');
%SetProperty(Script.prototype,'constructor',Script,2);
%SetCode(Script,function(a){

	throw new $Error("Not supported");
});



function FormatMessage(a){
	if(kMessages===0){
		kMessages={

			cyclic_proto:"Cyclic __proto__ value",

			unexpected_token:"Unexpected token %0",
			unexpected_token_number:"Unexpected number",
			unexpected_token_string:"Unexpected string",
			unexpected_token_identifier:"Unexpected identifier",
			unexpected_eos:"Unexpected end of input",
			malformed_regexp:"Invalid regular expression: /%0/: %1",
			unterminated_regexp:"Invalid regular expression: missing /",
			regexp_flags:"Cannot supply flags when constructing one RegExp from another",
			incompatible_method_receiver:"Method %0 called on incompatible receiver %1",
			invalid_lhs_in_assignment:"Invalid left-hand side in assignment",
			invalid_lhs_in_for_in:"Invalid left-hand side in for-in",
			invalid_lhs_in_postfix_op:"Invalid left-hand side expression in postfix operation",
			invalid_lhs_in_prefix_op:"Invalid left-hand side expression in prefix operation",
			multiple_defaults_in_switch:"More than one default clause in switch statement",
			newline_after_throw:"Illegal newline after throw",
			redeclaration:"%0 '%1' has already been declared",
			no_catch_or_finally:"Missing catch or finally after try",
			unknown_label:"Undefined label '%0'",
			uncaught_exception:"Uncaught %0",
			stack_trace:"Stack Trace:\n%0",
			called_non_callable:"%0 is not a function",
			undefined_method:"Object %1 has no method '%0'",
			property_not_function:"Property '%0' of object %1 is not a function",
			cannot_convert_to_primitive:"Cannot convert object to primitive value",
			not_constructor:"%0 is not a constructor",
			not_defined:"%0 is not defined",
			non_object_property_load:"Cannot read property '%0' of %1",
			non_object_property_store:"Cannot set property '%0' of %1",
			non_object_property_call:"Cannot call method '%0' of %1",
			with_expression:"%0 has no properties",
			illegal_invocation:"Illegal invocation",
			no_setter_in_callback:"Cannot set property %0 of %1 which has only a getter",
			apply_non_function:"Function.prototype.apply was called on %0, which is a %1 and not a function",
			apply_wrong_args:"Function.prototype.apply: Arguments list has wrong type",
			invalid_in_operator_use:"Cannot use 'in' operator to search for '%0' in %1",
			instanceof_function_expected:"Expecting a function in instanceof check, but got %0",
			instanceof_nonobject_proto:"Function has non-object prototype '%0' in instanceof check",
			null_to_object:"Cannot convert null to object",
			reduce_no_initial:"Reduce of empty array with no initial value",
			getter_must_be_callable:"Getter must be a function: %0",
			setter_must_be_callable:"Setter must be a function: %0",
			value_and_accessor:"Invalid property.  A property cannot both have accessors and be writable or have a value: %0",
			proto_object_or_null:"Object prototype may only be an Object or null",
			property_desc_object:"Property description must be an object: %0",
			redefine_disallowed:"Cannot redefine property: %0",
			define_disallowed:"Cannot define property, object is not extensible: %0",

			invalid_array_length:"Invalid array length",
			stack_overflow:"Maximum call stack size exceeded",

			unable_to_parse:"Parse error",
			duplicate_regexp_flag:"Duplicate RegExp flag %0",
			invalid_regexp:"Invalid RegExp pattern /%0/",
			illegal_break:"Illegal break statement",
			illegal_continue:"Illegal continue statement",
			illegal_return:"Illegal return statement",
			error_loading_debugger:"Error loading debugger",
			no_input_to_regexp:"No input to %0",
			result_not_primitive:"Result of %0 must be a primitive, was %1",
			invalid_json:"String '%0' is not valid JSON",
			circular_structure:"Converting circular structure to JSON",
			obj_ctor_property_non_object:"Object.%0 called on non-object",
			array_indexof_not_defined:"Array.getIndexOf: Argument undefined",
			object_not_extensible:"Can't add property %0, object is not extensible",
			illegal_access:"Illegal access",
			invalid_preparser_data:"Invalid preparser data for function %0"
		};
	}
	var b=kMessages[a.type];
	if(!b)return"<unknown message "+a.type+">";
	return FormatString(b,a.args);
}


function GetLineNumber(a){
	if(a.startPos==-1)return kNoLineNumberInfo;
	var b=a.script.locationFromPosition(a.startPos,true);
	if(b==null)return kNoLineNumberInfo;
	return b.line+1;
}




function GetSourceLine(a){
	var b=a.script.locationFromPosition(a.startPos,true);
	if(b==null)return"";
	b.restrict();
	return b.sourceText();
}


function MakeTypeError(a,b){
	return MakeGenericError($TypeError,a,b);
}


function MakeRangeError(a,b){
	return MakeGenericError($RangeError,a,b);
}


function MakeSyntaxError(a,b){
	return MakeGenericError($SyntaxError,a,b);
}


function MakeReferenceError(a,b){
	return MakeGenericError($ReferenceError,a,b);
}


function MakeEvalError(a,b){
	return MakeGenericError($EvalError,a,b);
}


function MakeError(a,b){
	return MakeGenericError($Error,a,b);
}







Script.prototype.lineFromPosition=function(a){
	var b=0;
	var d=this.lineCount()-1;
	var g=this.line_ends;


	if(a>g[d]){
		return-1;
	}


	if(a<=g[0]){
		return 0;
	}


	while(d>=1){
		var h=(b+d)>>1;

		if(a>g[h]){
			b=h+1;
		}else if(a<=g[h-1]){
			d=h-1;
		}else{
			return h;
		}
	}
	return-1;
}









Script.prototype.locationFromPosition=function(position,
		include_resource_offset){
			var a=this.lineFromPosition(position);
			if(a==-1)return null;


			var b=this.line_ends;
			var d=a==0?0:b[a-1]+1;
			var g=b[a];
			if(g>0&&StringCharAt.call(this.source,g-1)=='\r')g--;
			var h=position-d;


			if(include_resource_offset){
				a+=this.line_offset;
				if(a==this.line_offset){
					h+=this.column_offset;
				}
			}

			return new SourceLocation(this,position,a,h,d,g);
		};
















Script.prototype.locationFromLine=function(a,b,d){


	var g=0;
	if(!(typeof(a)==='undefined')){
		g=a-this.line_offset;
	}



	var h=b||0;
	if(g==0){
		h-=this.column_offset
	}

	var i=d||0;
	if(g<0||h<0||i<0)return null;
	if(g==0){
		return this.locationFromPosition(i+h,false);
	}else{

		var j=this.lineFromPosition(i);

		if(j==-1||j+g>=this.lineCount()){
			return null;
		}

		return this.locationFromPosition(this.line_ends[j+g-1]+1+h);
	}
}












Script.prototype.sourceSlice=function(a,b){
	var d=(typeof(a)==='undefined')?this.line_offset:a;
	var g=(typeof(b)==='undefined')?this.line_offset+this.lineCount():b


		d-=this.line_offset;
	g-=this.line_offset;
	if(d<0)d=0;
	if(g>this.lineCount())g=this.lineCount();


	if(d>=this.lineCount()||
			g<0||
			d>g){
				return null;
			}

	var h=this.line_ends;
	var i=d==0?0:h[d-1]+1;
	var j=g==0?0:h[g-1]+1;


	return new SourceSlice(this,d+this.line_offset,g+this.line_offset,
			i,j);
}


Script.prototype.sourceLine=function(a){


	var b=0;
	if(!(typeof(a)==='undefined')){
		b=a-this.line_offset;
	}


	if(b<0||this.lineCount()<=b){
		return null;
	}


	var d=this.line_ends;
	var g=b==0?0:d[b-1]+1;
	var h=d[b];
	return StringSubstring.call(this.source,g,h);
}







Script.prototype.lineCount=function(){

	return this.line_ends.length;
};












Script.prototype.nameOrSourceURL=function(){
	if(this.name)
		return this.name;




	var a=
		/\/\/@[\040\t]sourceURL=[\040\t]*([^\s'"]*)[\040\t]*$/m;
	var b=a.exec(this.source);
	return b?b[1]:this.name;
}























function SourceLocation(a,b,d,g,h,i){
	this.script=a;
	this.position=b;
	this.line=d;
	this.column=g;
	this.start=h;
	this.end=i;
}


const kLineLengthLimit=78;









SourceLocation.prototype.restrict=function(a,b){

	var d;
	var g;
	if(!(typeof(a)==='undefined')){
		d=a;
	}else{
		d=kLineLengthLimit;
	}
	if(!(typeof(b)==='undefined')){
		g=b;
	}else{


		if(d<=20){
			g=$floor(d/2);
		}else{
			g=d-10;
		}
	}
	if(g>=d){
		g=d-1;
	}




	if(this.end-this.start>d){
		var h=this.position-g;
		var i=this.position+d-g;
		if(this.start<h&&i<this.end){
			this.start=h;
			this.end=i;
		}else if(this.start<h){
			this.start=this.end-d;
		}else{
			this.end=this.start+d;
		}
	}
};







SourceLocation.prototype.sourceText=function(){
	return StringSubstring.call(this.script.source,this.start,this.end);
};




















function SourceSlice(a,b,d,g,h){
	this.script=a;
	this.from_line=b;
	this.to_line=d;
	this.from_position=g;
	this.to_position=h;
}







SourceSlice.prototype.sourceText=function(){
	return StringSubstring.call(this.script.source,this.from_position,this.to_position);
};




function GetPositionInLine(a){
	var b=a.script.locationFromPosition(a.startPos,false);
	if(b==null)return-1;
	b.restrict();
	return a.startPos-b.start;
}


function ErrorMessage(type,args,startPos,endPos,script,stackTrace,
		stackFrames){
			this.startPos=startPos;
			this.endPos=endPos;
			this.type=type;
			this.args=args;
			this.script=script;
			this.stackTrace=stackTrace;
			this.stackFrames=stackFrames;
		}


function MakeMessage(type,args,startPos,endPos,script,stackTrace,
		stackFrames){
			return new ErrorMessage(type,args,startPos,endPos,script,stackTrace,
					stackFrames);
		}


function GetStackTraceLine(a,b,d,g){
	return FormatSourcePosition(new CallSite(a,b,d));
}






function DefineOneShotAccessor(a,b,d){



	var g=false;
	var h;
	a.__defineGetter__(b,function(){
		if(g){
			return h;
		}
		g=true;
		h=d(a);
		return h;
	});
	a.__defineSetter__(b,function(i){
		g=true;
		h=i;
	});
}

function CallSite(a,b,d){
	this.receiver=a;
	this.fun=b;
	this.pos=d;
}

CallSite.prototype.getThis=function(){
	return this.receiver;
};

CallSite.prototype.getTypeName=function(){
	var a=this.receiver.constructor;
	if(!a)
		return $Object.prototype.toString.call(this.receiver);
	var b=a.name;
	if(!b)
		return $Object.prototype.toString.call(this.receiver);
	return b;
};

CallSite.prototype.isToplevel=function(){
	if(this.receiver==null)
		return true;
	return(%_ClassOf(this.receiver)==='global');
};

CallSite.prototype.isEval=function(){
	var a=%FunctionGetScript(this.fun);
	return a&&a.compilation_type==COMPILATION_TYPE_EVAL;
};

CallSite.prototype.getEvalOrigin=function(){
	var a=%FunctionGetScript(this.fun);
	return FormatEvalOrigin(a);
};

CallSite.prototype.getScriptNameOrSourceURL=function(){
	var a=%FunctionGetScript(this.fun);
	return a?a.nameOrSourceURL():null;
};

CallSite.prototype.getFunction=function(){
	return this.fun;
};

CallSite.prototype.getFunctionName=function(){

	var a=this.fun.name;
	if(a){
		return a;
	}else{
		return %FunctionGetInferredName(this.fun);
	}

	var b=%FunctionGetScript(this.fun);
	if(b&&b.compilation_type==COMPILATION_TYPE_EVAL)
		return"eval";
	return null;
};

CallSite.prototype.getMethodName=function(){


	var a=this.fun.name;
	if(a&&this.receiver&&
			(ObjectLookupGetter.call(this.receiver,a)===this.fun||
			 ObjectLookupSetter.call(this.receiver,a)===this.fun||
			 this.receiver[a]===this.fun)){


				 return a;
			 }
	var b=null;
	for(var d in this.receiver){
		if(this.receiver.__lookupGetter__(d)===this.fun||
				this.receiver.__lookupSetter__(d)===this.fun||
				(!this.receiver.__lookupGetter__(d)&&this.receiver[d]===this.fun)){

					if(b)
						return null;
					b=d;
				}
	}
	if(b)
		return b;
	return null;
};

CallSite.prototype.getFileName=function(){
	var a=%FunctionGetScript(this.fun);
	return a?a.name:null;
};

CallSite.prototype.getLineNumber=function(){
	if(this.pos==-1)
		return null;
	var a=%FunctionGetScript(this.fun);
	var b=null;
	if(a){
		b=a.locationFromPosition(this.pos,true);
	}
	return b?b.line+1:null;
};

CallSite.prototype.getColumnNumber=function(){
	if(this.pos==-1)
		return null;
	var a=%FunctionGetScript(this.fun);
	var b=null;
	if(a){
		b=a.locationFromPosition(this.pos,true);
	}
	return b?b.column+1:null;
};

CallSite.prototype.isNative=function(){
	var a=%FunctionGetScript(this.fun);
	return a?(a.type==TYPE_NATIVE):false;
};

CallSite.prototype.getPosition=function(){
	return this.pos;
};

CallSite.prototype.isConstructor=function(){
	var a=this.receiver?this.receiver.constructor:null;
	if(!a)
		return false;
	return this.fun===a;
};

function FormatEvalOrigin(a){
	var b=a.nameOrSourceURL();
	if(b)
		return b;

	var d="eval at ";
	if(a.eval_from_function_name){
		d+=a.eval_from_function_name;
	}else{
		d+="<anonymous>";
	}

	var g=a.eval_from_script;
	if(g){
		if(g.compilation_type==COMPILATION_TYPE_EVAL){

			d+=" ("+FormatEvalOrigin(g)+")";
		}else{

			if(g.name){
				d+=" ("+g.name;
				var h=g.locationFromPosition(a.eval_from_script_position,true);
				if(h){
					d+=":"+(h.line+1);
					d+=":"+(h.column+1);
				}
				d+=")"
			}else{
				d+=" (unknown source)";
			}
		}
	}

	return d;
};

function FormatSourcePosition(a){
	var b;
	var d="";
	if(a.isNative()){
		d="native";
	}else if(a.isEval()){
		b=a.getScriptNameOrSourceURL();
		if(!b)
			d=a.getEvalOrigin();
	}else{
		b=a.getFileName();
	}

	if(b){
		d+=b;
		var g=a.getLineNumber();
		if(g!=null){
			d+=":"+g;
			var h=a.getColumnNumber();
			if(h){
				d+=":"+h;
			}
		}
	}

	if(!d){
		d="unknown source";
	}
	var i="";
	var j=a.getFunction().name;
	var k=true;
	var l=a.isConstructor();
	var m=!(a.isToplevel()||l);
	if(m){
		var o=a.getMethodName();
		i+=a.getTypeName()+".";
		if(j){
			i+=j;
			if(o&&(o!=j)){
				i+=" [as "+o+"]";
			}
		}else{
			i+=o||"<anonymous>";
		}
	}else if(l){
		i+="new "+(j||"<anonymous>");
	}else if(j){
		i+=j;
	}else{
		i+=d;
		k=false;
	}
	if(k){
		i+=" ("+d+")";
	}
	return i;
}

function FormatStackTrace(a,b){
	var d=[];
	try{
		d.push(a.toString());
	}catch(e){
		try{
			d.push("<error: "+e+">");
		}catch(ee){
			d.push("<error>");
		}
	}
	for(var g=0;g<b.length;g++){
		var h=b[g];
		var i;
		try{
			i=FormatSourcePosition(h);
		}catch(e){
			try{
				i="<error: "+e+">";
			}catch(ee){

				i="<error>";
			}
		}
		d.push("    at "+i);
	}
	return d.join("\n");
}

function FormatRawStackTrace(a,b){
	var d=[];
	for(var g=0;g<b.length;g+=3){
		var h=b[g];
		var i=b[g+1];
		var j=b[g+2];
		var k=%FunctionGetPositionForOffset(i,j);
		d.push(new CallSite(h,i,k));
	}
	if((%_IsFunction($Error.prepareStackTrace))){
		return $Error.prepareStackTrace(a,d);
	}else{
		return FormatStackTrace(a,d);
	}
}

function DefineError(a){






	var b=a.name;
	%SetProperty(global,b,a,2);
	this['$'+b]=a;

	if(b=='Error'){




		function ErrorPrototype(){}
		%FunctionSetPrototype(ErrorPrototype,$Object.prototype);
		%FunctionSetInstanceClassName(ErrorPrototype,'Error');
		%FunctionSetPrototype(a,new ErrorPrototype());
	}else{
		%FunctionSetPrototype(a,new $Error());
	}
	%FunctionSetInstanceClassName(a,'Error');
	%SetProperty(a.prototype,'constructor',a,2);
	a.prototype.name=b;
	%SetCode(a,function(d){
		if(%_IsConstructCall()){
			if(d===kAddMessageAccessorsMarker){
				DefineOneShotAccessor(this,'message',function(g){
					return FormatMessage({type:g.type,args:g.arguments});
				});
			}else if(!(typeof(d)==='undefined')){
				this.message=ToString(d);
			}
			captureStackTrace(this,a);
		}else{
			return new a(d);
		}
	});
}

function captureStackTrace(a,b){
	var d=$Error.stackTraceLimit;
	if(!d)return;
	if(d<0||d>10000)
		d=10000;
	var g=%CollectStackTrace(b?b:captureStackTrace,
			d);
	DefineOneShotAccessor(a,'stack',function(a){
		return FormatRawStackTrace(a,g);
	});
};

$Math.__proto__=global.Object.prototype;

DefineError(function Error(){});
DefineError(function TypeError(){});
DefineError(function RangeError(){});
DefineError(function SyntaxError(){});
DefineError(function ReferenceError(){});
DefineError(function EvalError(){});
DefineError(function URIError(){});

$Error.captureStackTrace=captureStackTrace;


$Error.prototype.message='';

%SetProperty($Error.prototype,'toString',function toString(){
	var a=this.type;
	if(a&&!this.hasOwnProperty("message")){
		return this.name+": "+FormatMessage({type:a,args:this.arguments});
	}
	var b=this.message;
	return this.name+(b?(": "+b):"");
},2);




const kStackOverflowBoilerplate=MakeRangeError('stack_overflow',[]);

































function CreateDate(a){
	var b=new $Date();
	b.setTime(a);
	return b;
}


const kApiFunctionCache={};
const functionCache=kApiFunctionCache;


function Instantiate(a,b){
	if(!%IsTemplate(a))return a;
	var d=%GetTemplateField(a,0);
	switch(d){
		case 0:
			return InstantiateFunction(a,b);
		case 1:
			var g=%GetTemplateField(a,2);
			var h=g?new(Instantiate(g))():{};
			ConfigureTemplateInstance(h,a);
			h=%ToFastProperties(h);
			return h;
		default:
			throw'Unknown API tag <'+d+'>';
	}
}


function InstantiateFunction(a,b){


	var d=kApiFunctionCache;
	var g=%GetTemplateField(a,2);
	var h=
		(g in d)&&(d[g]!=-1);
	if(!h){
		try{
			d[g]=null;
			var i=%CreateApiFunction(a);
			if(b)%FunctionSetName(i,b);
			d[g]=i;
			var j=%GetTemplateField(a,5);
			i.prototype=j?Instantiate(j):{};
			%SetProperty(i.prototype,"constructor",i,2);
			var k=%GetTemplateField(a,6);
			if(k){
				var l=Instantiate(k);
				i.prototype.__proto__=l.prototype;
			}
			ConfigureTemplateInstance(i,a);
		}catch(e){
			d[g]=-1;
			throw e;
		}
	}
	return d[g];
}


function ConfigureTemplateInstance(a,b){
	var d=%GetTemplateField(b,1);
	if(d){

		var g=%DisableAccessChecks(a);
		try{
			for(var h=0;h<d[0];h+=3){
				var i=d[h+1];
				var j=d[h+2];
				var k=d[h+3];
				var l=Instantiate(j,i);
				%SetProperty(a,i,l,k);
			}
		}finally{
			if(g)%EnableAccessChecks(a);
		}
	}
}





























const kDefaultBacktraceLength=10;

const Debug={};




const sourceLineBeginningSkip=/^(?:\s*(?:\/\*.*?\*\/)*)*/;



Debug.DebugEvent={Break:1,
	Exception:2,
	NewFunction:3,
	BeforeCompile:4,
	AfterCompile:5,
	ScriptCollected:6};


Debug.ExceptionBreak={Caught:0,
	Uncaught:1};


Debug.StepAction={StepOut:0,
	StepNext:1,
	StepIn:2,
	StepMin:3,
	StepInMin:4};


Debug.ScriptType={Native:0,
	Extension:1,
	Normal:2};



Debug.ScriptCompilationType={Host:0,
	Eval:1,
	JSON:2};


Debug.ScriptBreakPointType={ScriptId:0,
	ScriptName:1};

function ScriptTypeFlag(a){
	return(1<<a);
}


var next_response_seq=0;
var next_break_point_number=1;
var break_points=[];
var script_break_points=[];
var debugger_flags={
	breakPointsActive:{
		value:true,
		getValue:function(){return this.value;},
		setValue:function(a){
			this.value=!!a;
			%SetDisableBreak(!this.value);
		}
	},
	breakOnCaughtException:{
		getValue:function(){return Debug.isBreakOnException();},
		setValue:function(a){
			if(a){
				Debug.setBreakOnException();
			}else{
				Debug.clearBreakOnException();
			}
		}
	},
	breakOnUncaughtException:{
		getValue:function(){return Debug.isBreakOnUncaughtException();},
		setValue:function(a){
			if(a){
				Debug.setBreakOnUncaughtException();
			}else{
				Debug.clearBreakOnUncaughtException();
			}
		}
	},
};



function MakeBreakPoint(a,b,d,g){
	var h=new BreakPoint(a,b,d,g);
	break_points.push(h);
	return h;
}






function BreakPoint(a,b,d,g){
	this.source_position_=a;
	this.source_line_=b;
	this.source_column_=d;
	if(g){
		this.script_break_point_=g;
	}else{
		this.number_=next_break_point_number++;
	}
	this.hit_count_=0;
	this.active_=true;
	this.condition_=null;
	this.ignoreCount_=0;
}


BreakPoint.prototype.number=function(){
	return this.number_;
};


BreakPoint.prototype.func=function(){
	return this.func_;
};


BreakPoint.prototype.source_position=function(){
	return this.source_position_;
};


BreakPoint.prototype.hit_count=function(){
	return this.hit_count_;
};


BreakPoint.prototype.active=function(){
	if(this.script_break_point()){
		return this.script_break_point().active();
	}
	return this.active_;
};


BreakPoint.prototype.condition=function(){
	if(this.script_break_point()&&this.script_break_point().condition()){
		return this.script_break_point().condition();
	}
	return this.condition_;
};


BreakPoint.prototype.ignoreCount=function(){
	return this.ignoreCount_;
};


BreakPoint.prototype.script_break_point=function(){
	return this.script_break_point_;
};


BreakPoint.prototype.enable=function(){
	this.active_=true;
};


BreakPoint.prototype.disable=function(){
	this.active_=false;
};


BreakPoint.prototype.setCondition=function(a){
	this.condition_=a;
};


BreakPoint.prototype.setIgnoreCount=function(a){
	this.ignoreCount_=a;
};


BreakPoint.prototype.isTriggered=function(a){

	if(!this.active())return false;


	if(this.condition()){

		try{
			var b=a.frame(0).evaluate(this.condition());

			if(!(b instanceof ValueMirror)||!%ToBoolean(b.value_)){
				return false;
			}
		}catch(e){

			return false;
		}
	}


	this.hit_count_++;
	if(this.script_break_point_){
		this.script_break_point_.hit_count_++;
	}


	if(this.ignoreCount_>0){
		this.ignoreCount_--;
		return false;
	}


	return true;
};




function IsBreakPointTriggered(a,b){
	return b.isTriggered(MakeExecutionState(a));
}





function ScriptBreakPoint(type,script_id_or_name,opt_line,opt_column,
		opt_groupId){
			this.type_=type;
			if(type==Debug.ScriptBreakPointType.ScriptId){
				this.script_id_=script_id_or_name;
			}else{
				this.script_name_=script_id_or_name;
			}
			this.line_=opt_line||0;
			this.column_=opt_column;
			this.groupId_=opt_groupId;
			this.hit_count_=0;
			this.active_=true;
			this.condition_=null;
			this.ignoreCount_=0;
			this.break_points_=[];
		}



ScriptBreakPoint.prototype.cloneForOtherScript=function(a){
	var b=new ScriptBreakPoint(Debug.ScriptBreakPointType.ScriptId,
			a.id,this.line_,this.column_,this.groupId_);
	b.number_=next_break_point_number++;
	script_break_points.push(b);

	b.hit_count_=this.hit_count_;
	b.active_=this.active_;
	b.condition_=this.condition_;
	b.ignoreCount_=this.ignoreCount_;
	return b;
}


ScriptBreakPoint.prototype.number=function(){
	return this.number_;
};


ScriptBreakPoint.prototype.groupId=function(){
	return this.groupId_;
};


ScriptBreakPoint.prototype.type=function(){
	return this.type_;
};


ScriptBreakPoint.prototype.script_id=function(){
	return this.script_id_;
};


ScriptBreakPoint.prototype.script_name=function(){
	return this.script_name_;
};


ScriptBreakPoint.prototype.line=function(){
	return this.line_;
};


ScriptBreakPoint.prototype.column=function(){
	return this.column_;
};


ScriptBreakPoint.prototype.actual_locations=function(){
	var a=[];
	for(var b=0;b<this.break_points_.length;b++){
		a.push(this.break_points_[b].actual_location);
	}
	return a;
}


ScriptBreakPoint.prototype.update_positions=function(a,b){
	this.line_=a;
	this.column_=b;
}


ScriptBreakPoint.prototype.hit_count=function(){
	return this.hit_count_;
};


ScriptBreakPoint.prototype.active=function(){
	return this.active_;
};


ScriptBreakPoint.prototype.condition=function(){
	return this.condition_;
};


ScriptBreakPoint.prototype.ignoreCount=function(){
	return this.ignoreCount_;
};


ScriptBreakPoint.prototype.enable=function(){
	this.active_=true;
};


ScriptBreakPoint.prototype.disable=function(){
	this.active_=false;
};


ScriptBreakPoint.prototype.setCondition=function(a){
	this.condition_=a;
};


ScriptBreakPoint.prototype.setIgnoreCount=function(a){
	this.ignoreCount_=a;


	for(var b=0;b<this.break_points_.length;b++){
		this.break_points_[b].setIgnoreCount(a);
	}
};




ScriptBreakPoint.prototype.matchesScript=function(a){
	if(this.type_==Debug.ScriptBreakPointType.ScriptId){
		return this.script_id_==a.id;
	}else{
		return this.script_name_==a.nameOrSourceURL()&&
			a.line_offset<=this.line_&&
			this.line_<a.line_offset+a.lineCount();
	}
};



ScriptBreakPoint.prototype.set=function(a){
	var b=this.column();
	var d=this.line();



	if((typeof(b)==='undefined')){
		var g=a.sourceLine(this.line());


		if(!a.sourceColumnStart_){
			a.sourceColumnStart_=new Array(a.lineCount());
		}


		if((typeof(a.sourceColumnStart_[d])==='undefined')){
			a.sourceColumnStart_[d]=
				g.match(sourceLineBeginningSkip)[0].length;
		}
		b=a.sourceColumnStart_[d];
	}


	var h=Debug.findScriptSourcePosition(a,this.line(),b);



	if(h===null)return;


	break_point=MakeBreakPoint(h,this.line(),this.column(),this);
	break_point.setIgnoreCount(this.ignoreCount());
	var i=%SetScriptBreakPoint(a,h,break_point);
	if((typeof(i)==='undefined')){
		i=h;
	}
	var j=a.locationFromPosition(i,true);
	break_point.actual_location={line:j.line,
		column:j.column};
	this.break_points_.push(break_point);
	return break_point;
};



ScriptBreakPoint.prototype.clear=function(){
	var a=[];
	for(var b=0;b<break_points.length;b++){
		if(break_points[b].script_break_point()&&
				break_points[b].script_break_point()===this){
					%ClearBreakPoint(break_points[b]);
				}else{
					a.push(break_points[b]);
				}
	}
	break_points=a;
	this.break_points_=[];
};




function UpdateScriptBreakPoints(a){
	for(var b=0;b<script_break_points.length;b++){
		if(script_break_points[b].type()==Debug.ScriptBreakPointType.ScriptName&&
				script_break_points[b].matchesScript(a)){
					script_break_points[b].set(a);
				}
	}
}


function GetScriptBreakPoints(a){
	var b=[];
	for(var d=0;d<script_break_points.length;d++){
		if(script_break_points[d].matchesScript(a)){
			b.push(script_break_points[d]);
		}
	}
	return b;
}


Debug.setListener=function(a,b){
	if(!(%_IsFunction(a))&&!(typeof(a)==='undefined')&&!(a===null)){
		throw new Error('Parameters have wrong types.');
	}
	%SetDebugEventListener(a,b);
};


Debug.breakExecution=function(a){
	%Break();
};

Debug.breakLocations=function(a){
	if(!(%_IsFunction(a)))throw new Error('Parameters have wrong types.');
	return %GetBreakLocations(a);
};






Debug.findScript=function(a){
	if((%_IsFunction(a))){
		return %FunctionGetScript(a);
	}else if((%_IsRegExp(a))){
		var b=Debug.scripts();
		var d=null;
		var g=0;
		for(var h in b){
			var i=b[h];
			if(a.test(i.name)){
				d=i;
				g++;
			}
		}





		if(g==1){
			return d;
		}else{
			return undefined;
		}
	}else{
		return %GetScript(a);
	}
};





Debug.scriptSource=function(a){
	return this.findScript(a).source;
};

Debug.source=function(a){
	if(!(%_IsFunction(a)))throw new Error('Parameters have wrong types.');
	return %FunctionGetSourceCode(a);
};

Debug.disassemble=function(a){
	if(!(%_IsFunction(a)))throw new Error('Parameters have wrong types.');
	return %DebugDisassembleFunction(a);
};

Debug.disassembleConstructor=function(a){
	if(!(%_IsFunction(a)))throw new Error('Parameters have wrong types.');
	return %DebugDisassembleConstructor(a);
};

Debug.ExecuteInDebugContext=function(a,b){
	if(!(%_IsFunction(a)))throw new Error('Parameters have wrong types.');
	return %ExecuteInDebugContext(a,!!b);
};

Debug.sourcePosition=function(a){
	if(!(%_IsFunction(a)))throw new Error('Parameters have wrong types.');
	return %FunctionGetScriptSourcePosition(a);
};


Debug.findFunctionSourceLocation=function(a,b,d){
	var g=%FunctionGetScript(a);
	var h=%FunctionGetScriptSourcePosition(a);
	return g.locationFromLine(b,d,h);
}




Debug.findScriptSourcePosition=function(a,b,d){
	var g=a.locationFromLine(b,d);
	return g?g.position:null;
}


Debug.findBreakPoint=function(a,b){
	var d;
	for(var g=0;g<break_points.length;g++){
		if(break_points[g].number()==a){
			d=break_points[g];

			if(b){
				break_points.splice(g,1);
			}
			break;
		}
	}
	if(d){
		return d;
	}else{
		return this.findScriptBreakPoint(a,b);
	}
};

Debug.findBreakPointActualLocations=function(a){
	for(var b=0;b<script_break_points.length;b++){
		if(script_break_points[b].number()==a){
			return script_break_points[b].actual_locations();
		}
	}
	for(var b=0;b<break_points.length;b++){
		if(break_points[b].number()==a){
			return[break_points[b].actual_location];
		}
	}
	return[];
}

Debug.setBreakPoint=function(a,b,d,g){
	if(!(%_IsFunction(a)))throw new Error('Parameters have wrong types.');

	if(%FunctionIsAPIFunction(a)){
		throw new Error('Cannot set break point in native code.');
	}

	var h=
		this.findFunctionSourceLocation(a,b,d).position;
	var i=h-this.sourcePosition(a);

	var j=%FunctionGetScript(a);

	if(j.type==Debug.ScriptType.Native){
		throw new Error('Cannot set break point in native code.');
	}


	if(j&&j.id){

		i+=%FunctionGetScriptSourcePosition(a);


		var k=j.locationFromPosition(i,false);
		return this.setScriptBreakPointById(j.id,
				k.line,k.column,
				g);
	}else{

		var l=MakeBreakPoint(i,b,d);
		var m=
			%SetFunctionBreakPoint(a,i,l);
		m+=this.sourcePosition(a);
		var o=j.locationFromPosition(m,true);
		l.actual_location={line:o.line,
			column:o.column};
		l.setCondition(g);
		return l.number();
	}
};


Debug.enableBreakPoint=function(a){
	var b=this.findBreakPoint(a,false);
	b.enable();
};


Debug.disableBreakPoint=function(a){
	var b=this.findBreakPoint(a,false);
	b.disable();
};


Debug.changeBreakPointCondition=function(a,b){
	var d=this.findBreakPoint(a,false);
	d.setCondition(b);
};


Debug.changeBreakPointIgnoreCount=function(a,b){
	if(b<0){
		throw new Error('Invalid argument');
	}
	var d=this.findBreakPoint(a,false);
	d.setIgnoreCount(b);
};


Debug.clearBreakPoint=function(a){
	var b=this.findBreakPoint(a,true);
	if(b){
		return %ClearBreakPoint(b);
	}else{
		b=this.findScriptBreakPoint(a,true);
		if(!b){
			throw new Error('Invalid breakpoint');
		}
	}
};


Debug.clearAllBreakPoints=function(){
	for(var a=0;a<break_points.length;a++){
		break_point=break_points[a];
		%ClearBreakPoint(break_point);
	}
	break_points=[];
};


Debug.findScriptBreakPoint=function(a,b){
	var d;
	for(var g=0;g<script_break_points.length;g++){
		if(script_break_points[g].number()==a){
			d=script_break_points[g];

			if(b){
				d.clear();
				script_break_points.splice(g,1);
			}
			break;
		}
	}
	return d;
}




Debug.setScriptBreakPoint=function(type,script_id_or_name,
		opt_line,opt_column,opt_condition,
		opt_groupId){

			var a=
				new ScriptBreakPoint(type,script_id_or_name,opt_line,opt_column,
						opt_groupId);


			a.number_=next_break_point_number++;
			a.setCondition(opt_condition);
			script_break_points.push(a);



			var b=this.scripts();
			for(var d=0;d<b.length;d++){
				if(a.matchesScript(b[d])){
					a.set(b[d]);
				}
			}

			return a.number();
		}


Debug.setScriptBreakPointById=function(script_id,
		opt_line,opt_column,
		opt_condition,opt_groupId){
			return this.setScriptBreakPoint(Debug.ScriptBreakPointType.ScriptId,
					script_id,opt_line,opt_column,
					opt_condition,opt_groupId);
		}


Debug.setScriptBreakPointByName=function(script_name,
		opt_line,opt_column,
		opt_condition,opt_groupId){
			return this.setScriptBreakPoint(Debug.ScriptBreakPointType.ScriptName,
					script_name,opt_line,opt_column,
					opt_condition,opt_groupId);
		}


Debug.enableScriptBreakPoint=function(a){
	var b=this.findScriptBreakPoint(a,false);
	b.enable();
};


Debug.disableScriptBreakPoint=function(a){
	var b=this.findScriptBreakPoint(a,false);
	b.disable();
};


Debug.changeScriptBreakPointCondition=function(a,b){
	var d=this.findScriptBreakPoint(a,false);
	d.setCondition(b);
};


Debug.changeScriptBreakPointIgnoreCount=function(a,b){
	if(b<0){
		throw new Error('Invalid argument');
	}
	var d=this.findScriptBreakPoint(a,false);
	d.setIgnoreCount(b);
};


Debug.scriptBreakPoints=function(){
	return script_break_points;
}


Debug.clearStepping=function(){
	%ClearStepping();
}

Debug.setBreakOnException=function(){
	return %ChangeBreakOnException(Debug.ExceptionBreak.Caught,true);
};

Debug.clearBreakOnException=function(){
	return %ChangeBreakOnException(Debug.ExceptionBreak.Caught,false);
};

Debug.isBreakOnException=function(){
	return!!%IsBreakOnException(Debug.ExceptionBreak.Caught);
};

Debug.setBreakOnUncaughtException=function(){
	return %ChangeBreakOnException(Debug.ExceptionBreak.Uncaught,true);
};

Debug.clearBreakOnUncaughtException=function(){
	return %ChangeBreakOnException(Debug.ExceptionBreak.Uncaught,false);
};

Debug.isBreakOnUncaughtException=function(){
	return!!%IsBreakOnException(Debug.ExceptionBreak.Uncaught);
};

Debug.showBreakPoints=function(a,b){
	if(!(%_IsFunction(a)))throw new Error('Parameters have wrong types.');
	var d=b?this.scriptSource(a):this.source(a);
	var g=b?this.sourcePosition(a):0;
	var h=this.breakLocations(a);
	if(!h)return d;
	h.sort(function(i,j){return i-j;});
	var k="";
	var l=0;
	var m;
	for(var o=0;o<h.length;o++){
		m=h[o]-g;
		k+=d.slice(l,m);
		k+="[B"+o+"]";
		l=m;
	}
	m=d.length;
	k+=d.substring(l,m);
	return k;
};




Debug.scripts=function(){

	return %DebugGetLoadedScripts();
};


Debug.debuggerFlags=function(){
	return debugger_flags;
};


function MakeExecutionState(a){
	return new ExecutionState(a);
}

function ExecutionState(a){
	this.break_id=a;
	this.selected_frame=0;
}

ExecutionState.prototype.prepareStep=function(a,b){
	var d=Debug.StepAction.StepIn;
	if(!(typeof(a)==='undefined'))d=%ToNumber(a);
	var g=b?%ToNumber(b):1;

	return %PrepareStep(this.break_id,d,g);
}

ExecutionState.prototype.evaluateGlobal=function(a,b){
	return MakeMirror(
			%DebugEvaluateGlobal(this.break_id,a,Boolean(b)));
};

ExecutionState.prototype.frameCount=function(){
	return %GetFrameCount(this.break_id);
};

ExecutionState.prototype.threadCount=function(){
	return %GetThreadCount(this.break_id);
};

ExecutionState.prototype.frame=function(a){

	if(a==null)a=this.selected_frame;
	if(a<0||a>=this.frameCount())
		throw new Error('Illegal frame index.');
	return new FrameMirror(this.break_id,a);
};

ExecutionState.prototype.cframesValue=function(a,b){
	return %GetCFrames(this.break_id);
};

ExecutionState.prototype.setSelectedFrame=function(a){
	var b=%ToNumber(a);
	if(b<0||b>=this.frameCount())throw new Error('Illegal frame index.');
	this.selected_frame=b;
};

ExecutionState.prototype.selectedFrame=function(){
	return this.selected_frame;
};

ExecutionState.prototype.debugCommandProcessor=function(a){
	return new DebugCommandProcessor(this,a);
};


function MakeBreakEvent(a,b){
	return new BreakEvent(a,b);
}


function BreakEvent(a,b){
	this.exec_state_=a;
	this.break_points_hit_=b;
}


BreakEvent.prototype.executionState=function(){
	return this.exec_state_;
};


BreakEvent.prototype.eventType=function(){
	return Debug.DebugEvent.Break;
};


BreakEvent.prototype.func=function(){
	return this.exec_state_.frame(0).func();
};


BreakEvent.prototype.sourceLine=function(){
	return this.exec_state_.frame(0).sourceLine();
};


BreakEvent.prototype.sourceColumn=function(){
	return this.exec_state_.frame(0).sourceColumn();
};


BreakEvent.prototype.sourceLineText=function(){
	return this.exec_state_.frame(0).sourceLineText();
};


BreakEvent.prototype.breakPointsHit=function(){
	return this.break_points_hit_;
};


BreakEvent.prototype.toJSONProtocol=function(){
	var a={seq:next_response_seq++,
		type:"event",
		event:"break",
		body:{invocationText:this.exec_state_.frame(0).invocationText(),
		}
	};


	var b=this.func().script();
	if(b){
		a.body.sourceLine=this.sourceLine(),
			a.body.sourceColumn=this.sourceColumn(),
				a.body.sourceLineText=this.sourceLineText(),
				a.body.script=MakeScriptObject_(b,false);
	}


	if(this.breakPointsHit()){
		a.body.breakpoints=[];
		for(var d=0;d<this.breakPointsHit().length;d++){


			var g=this.breakPointsHit()[d];
			var h=g.script_break_point();
			var i;
			if(h){
				i=h.number();
			}else{
				i=g.number();
			}
			a.body.breakpoints.push(i);
		}
	}
	return JSON.stringify(ObjectToProtocolObject_(a));
};


function MakeExceptionEvent(a,b,d){
	return new ExceptionEvent(a,b,d);
}


function ExceptionEvent(a,b,d){
	this.exec_state_=a;
	this.exception_=b;
	this.uncaught_=d;
}


ExceptionEvent.prototype.executionState=function(){
	return this.exec_state_;
};


ExceptionEvent.prototype.eventType=function(){
	return Debug.DebugEvent.Exception;
};


ExceptionEvent.prototype.exception=function(){
	return this.exception_;
}


ExceptionEvent.prototype.uncaught=function(){
	return this.uncaught_;
}


ExceptionEvent.prototype.func=function(){
	return this.exec_state_.frame(0).func();
};


ExceptionEvent.prototype.sourceLine=function(){
	return this.exec_state_.frame(0).sourceLine();
};


ExceptionEvent.prototype.sourceColumn=function(){
	return this.exec_state_.frame(0).sourceColumn();
};


ExceptionEvent.prototype.sourceLineText=function(){
	return this.exec_state_.frame(0).sourceLineText();
};


ExceptionEvent.prototype.toJSONProtocol=function(){
	var a=new ProtocolMessage();
	a.event="exception";
	a.body={uncaught:this.uncaught_,
		exception:MakeMirror(this.exception_)
	};


	if(this.exec_state_.frameCount()>0){
		a.body.sourceLine=this.sourceLine();
		a.body.sourceColumn=this.sourceColumn();
		a.body.sourceLineText=this.sourceLineText();


		var b=this.func().script();
		if(b){
			a.body.script=MakeScriptObject_(b,false);
		}
	}else{
		a.body.sourceLine=-1;
	}

	return a.toJSONProtocol();
};


function MakeCompileEvent(a,b,d){
	return new CompileEvent(a,b,d);
}


function CompileEvent(a,b,d){
	this.exec_state_=a;
	this.script_=MakeMirror(b);
	this.before_=d;
}


CompileEvent.prototype.executionState=function(){
	return this.exec_state_;
};


CompileEvent.prototype.eventType=function(){
	if(this.before_){
		return Debug.DebugEvent.BeforeCompile;
	}else{
		return Debug.DebugEvent.AfterCompile;
	}
};


CompileEvent.prototype.script=function(){
	return this.script_;
};


CompileEvent.prototype.toJSONProtocol=function(){
	var a=new ProtocolMessage();
	a.running=true;
	if(this.before_){
		a.event="beforeCompile";
	}else{
		a.event="afterCompile";
	}
	a.body={};
	a.body.script=this.script_;

	return a.toJSONProtocol();
}


function MakeNewFunctionEvent(a){
	return new NewFunctionEvent(a);
}


function NewFunctionEvent(a){
	this.func=a;
}


NewFunctionEvent.prototype.eventType=function(){
	return Debug.DebugEvent.NewFunction;
};


NewFunctionEvent.prototype.name=function(){
	return this.func.name;
};


NewFunctionEvent.prototype.setBreakPoint=function(a){
	Debug.setBreakPoint(this.func,a||0);
};


function MakeScriptCollectedEvent(a,b){
	return new ScriptCollectedEvent(a,b);
}


function ScriptCollectedEvent(a,b){
	this.exec_state_=a;
	this.id_=b;
}


ScriptCollectedEvent.prototype.id=function(){
	return this.id_;
};


ScriptCollectedEvent.prototype.executionState=function(){
	return this.exec_state_;
};


ScriptCollectedEvent.prototype.toJSONProtocol=function(){
	var a=new ProtocolMessage();
	a.running=true;
	a.event="scriptCollected";
	a.body={};
	a.body.script={id:this.id()};
	return a.toJSONProtocol();
}


function MakeScriptObject_(a,b){
	var d={id:a.id(),
		name:a.name(),
		lineOffset:a.lineOffset(),
		columnOffset:a.columnOffset(),
		lineCount:a.lineCount(),
	};
	if(!(typeof(a.data())==='undefined')){
		d.data=a.data();
	}
	if(b){
		d.source=a.source();
	}
	return d;
};


function DebugCommandProcessor(a,b){
	this.exec_state_=a;
	this.running_=b||false;
};


DebugCommandProcessor.prototype.processDebugRequest=function(a){
	return this.processDebugJSONRequest(a);
}


function ProtocolMessage(a){

	this.seq=next_response_seq++;

	if(a){


		this.type='response';
		this.request_seq=a.seq;
		this.command=a.command;
	}else{

		this.type='event';
	}
	this.success=true;

	this.running=undefined;
}


ProtocolMessage.prototype.setOption=function(a,b){
	if(!this.options_){
		this.options_={};
	}
	this.options_[a]=b;
}


ProtocolMessage.prototype.failed=function(a){
	this.success=false;
	this.message=a;
}


ProtocolMessage.prototype.toJSONProtocol=function(){

	var a={};
	a.seq=this.seq;
	if(this.request_seq){
		a.request_seq=this.request_seq;
	}
	a.type=this.type;
	if(this.event){
		a.event=this.event;
	}
	if(this.command){
		a.command=this.command;
	}
	if(this.success){
		a.success=this.success;
	}else{
		a.success=false;
	}
	if(this.body){

		var b;
		var d=MakeMirrorSerializer(true,this.options_);
		if(this.body instanceof Mirror){
			b=d.serializeValue(this.body);
		}else if(this.body instanceof Array){
			b=[];
			for(var g=0;g<this.body.length;g++){
				if(this.body[g]instanceof Mirror){
					b.push(d.serializeValue(this.body[g]));
				}else{
					b.push(ObjectToProtocolObject_(this.body[g],d));
				}
			}
		}else{
			b=ObjectToProtocolObject_(this.body,d);
		}
		a.body=b;
		a.refs=d.serializeReferencedObjects();
	}
	if(this.message){
		a.message=this.message;
	}
	a.running=this.running;
	return JSON.stringify(a);
}


DebugCommandProcessor.prototype.createResponse=function(a){
	return new ProtocolMessage(a);
};


DebugCommandProcessor.prototype.processDebugJSONRequest=function(a){
	var b;
	var d;
	try{
		try{

			b=%CompileString('('+a+')',false)();


			d=this.createResponse(b);

			if(!b.type){
				throw new Error('Type not specified');
			}

			if(b.type!='request'){
				throw new Error("Illegal type '"+b.type+"' in request");
			}

			if(!b.command){
				throw new Error('Command not specified');
			}

			if(b.arguments){
				var g=b.arguments;


				if(g.inlineRefs||g.compactFormat){
					d.setOption('inlineRefs',true);
				}
				if(!(typeof(g.maxStringLength)==='undefined')){
					d.setOption('maxStringLength',g.maxStringLength);
				}
			}

			if(b.command=='continue'){
				this.continueRequest_(b,d);
			}else if(b.command=='break'){
				this.breakRequest_(b,d);
			}else if(b.command=='setbreakpoint'){
				this.setBreakPointRequest_(b,d);
			}else if(b.command=='changebreakpoint'){
				this.changeBreakPointRequest_(b,d);
			}else if(b.command=='clearbreakpoint'){
				this.clearBreakPointRequest_(b,d);
			}else if(b.command=='clearbreakpointgroup'){
				this.clearBreakPointGroupRequest_(b,d);
			}else if(b.command=='listbreakpoints'){
				this.listBreakpointsRequest_(b,d);
			}else if(b.command=='backtrace'){
				this.backtraceRequest_(b,d);
			}else if(b.command=='frame'){
				this.frameRequest_(b,d);
			}else if(b.command=='scopes'){
				this.scopesRequest_(b,d);
			}else if(b.command=='scope'){
				this.scopeRequest_(b,d);
			}else if(b.command=='evaluate'){
				this.evaluateRequest_(b,d);
			}else if(b.command=='lookup'){
				this.lookupRequest_(b,d);
			}else if(b.command=='references'){
				this.referencesRequest_(b,d);
			}else if(b.command=='source'){
				this.sourceRequest_(b,d);
			}else if(b.command=='scripts'){
				this.scriptsRequest_(b,d);
			}else if(b.command=='threads'){
				this.threadsRequest_(b,d);
			}else if(b.command=='suspend'){
				this.suspendRequest_(b,d);
			}else if(b.command=='version'){
				this.versionRequest_(b,d);
			}else if(b.command=='profile'){
				this.profileRequest_(b,d);
			}else if(b.command=='changelive'){
				this.changeLiveRequest_(b,d);
			}else if(b.command=='flags'){
				this.debuggerFlagsRequest_(b,d);
			}else{
				throw new Error('Unknown command "'+b.command+'" in request');
			}
		}catch(e){

			if(!d){
				d=this.createResponse();
			}
			d.success=false;
			d.message=%ToString(e);
		}


		try{
			if(!(typeof(d.running)==='undefined')){

				this.running_=d.running;
			}
			d.running=this.running_;
			return d.toJSONProtocol();
		}catch(e){

			return'{"seq":'+d.seq+','+
				'"request_seq":'+b.seq+','+
					'"type":"response",'+
					'"success":false,'+
					'"message":"Internal error: '+%ToString(e)+'"}';
		}
	}catch(e){

		return'{"seq":0,"type":"response","success":false,"message":"Internal error"}';
	}
};


DebugCommandProcessor.prototype.continueRequest_=function(a,b){

	if(a.arguments){
		var d=1;
		var g=Debug.StepAction.StepIn;


		var h=a.arguments.stepaction;
		var i=a.arguments.stepcount;


		if(i){
			d=%ToNumber(i);
			if(d<0){
				throw new Error('Invalid stepcount argument "'+i+'".');
			}
		}


		if(h){
			if(h=='in'){
				g=Debug.StepAction.StepIn;
			}else if(h=='min'){
				g=Debug.StepAction.StepMin;
			}else if(h=='next'){
				g=Debug.StepAction.StepNext;
			}else if(h=='out'){
				g=Debug.StepAction.StepOut;
			}else{
				throw new Error('Invalid stepaction argument "'+h+'".');
			}
		}


		this.exec_state_.prepareStep(g,d);
	}


	b.running=true;
};


DebugCommandProcessor.prototype.breakRequest_=function(a,b){

};


DebugCommandProcessor.prototype.setBreakPointRequest_=
function(a,b){

	if(!a.arguments){
		b.failed('Missing arguments');
		return;
	}


	var d=a.arguments.type;
	var g=a.arguments.target;
	var h=a.arguments.line;
	var i=a.arguments.column;
	var j=(typeof(a.arguments.enabled)==='undefined')?
		true:a.arguments.enabled;
	var k=a.arguments.condition;
	var l=a.arguments.ignoreCount;
	var m=a.arguments.groupId;


	if(!d||(typeof(g)==='undefined')){
		b.failed('Missing argument "type" or "target"');
		return;
	}
	if(d!='function'&&d!='handle'&&
			d!='script'&&d!='scriptId'){
				b.failed('Illegal type "'+d+'"');
				return;
			}


	var o;
	if(d=='function'){

		if(!(typeof(g)==='string')){
			b.failed('Argument "target" is not a string value');
			return;
		}
		var q;
		try{

			q=this.exec_state_.evaluateGlobal(g).value();
		}catch(e){
			b.failed('Error: "'+%ToString(e)+
					'" evaluating "'+g+'"');
			return;
		}
		if(!(%_IsFunction(q))){
			b.failed('"'+g+'" does not evaluate to a function');
			return;
		}


		o=Debug.setBreakPoint(q,h,i,k);
	}else if(d=='handle'){

		var r=parseInt(g,10);
		var t=LookupMirror(r);
		if(!t){
			return b.failed('Object #'+r+'# not found');
		}
		if(!t.isFunction()){
			return b.failed('Object #'+r+'# is not a function');
		}


		o=Debug.setBreakPoint(t.value(),
				h,i,k);
	}else if(d=='script'){

		o=
			Debug.setScriptBreakPointByName(g,h,i,k,
					m);
	}else{
		o=
			Debug.setScriptBreakPointById(g,h,i,k,m);
	}


	var u=Debug.findBreakPoint(o);
	if(l){
		Debug.changeBreakPointIgnoreCount(o,l);
	}
	if(!j){
		Debug.disableBreakPoint(o);
	}


	b.body={type:d,
		breakpoint:o}


	if(u instanceof ScriptBreakPoint){
		if(u.type()==Debug.ScriptBreakPointType.ScriptId){
			b.body.type='scriptId';
			b.body.script_id=u.script_id();
		}else{
			b.body.type='scriptName';
			b.body.script_name=u.script_name();
		}
		b.body.line=u.line();
		b.body.column=u.column();
		b.body.actual_locations=u.actual_locations();
	}else{
		b.body.type='function';
		b.body.actual_locations=[u.actual_location];
	}
};


DebugCommandProcessor.prototype.changeBreakPointRequest_=function(a,b){

	if(!a.arguments){
		b.failed('Missing arguments');
		return;
	}


	var d=%ToNumber(a.arguments.breakpoint);
	var g=a.arguments.enabled;
	var h=a.arguments.condition;
	var i=a.arguments.ignoreCount;


	if(!d){
		b.failed('Missing argument "breakpoint"');
		return;
	}


	if(!(typeof(g)==='undefined')){
		if(g){
			Debug.enableBreakPoint(d);
		}else{
			Debug.disableBreakPoint(d);
		}
	}


	if(!(typeof(h)==='undefined')){
		Debug.changeBreakPointCondition(d,h);
	}


	if(!(typeof(i)==='undefined')){
		Debug.changeBreakPointIgnoreCount(d,i);
	}
}


DebugCommandProcessor.prototype.clearBreakPointGroupRequest_=function(a,b){

	if(!a.arguments){
		b.failed('Missing arguments');
		return;
	}


	var d=a.arguments.groupId;


	if(!d){
		b.failed('Missing argument "groupId"');
		return;
	}

	var g=[];
	var h=[];
	for(var i=0;i<script_break_points.length;i++){
		var j=script_break_points[i];
		if(j.groupId()==d){
			g.push(j.number());
			j.clear();
		}else{
			h.push(j);
		}
	}
	script_break_points=h;


	b.body={breakpoints:g};
}


DebugCommandProcessor.prototype.clearBreakPointRequest_=function(a,b){

	if(!a.arguments){
		b.failed('Missing arguments');
		return;
	}


	var d=%ToNumber(a.arguments.breakpoint);


	if(!d){
		b.failed('Missing argument "breakpoint"');
		return;
	}


	Debug.clearBreakPoint(d);


	b.body={breakpoint:d}
}


DebugCommandProcessor.prototype.listBreakpointsRequest_=function(a,b){
	var d=[];
	for(var g=0;g<script_break_points.length;g++){
		var h=script_break_points[g];

		var i={
			number:h.number(),
			line:h.line(),
			column:h.column(),
			groupId:h.groupId(),
			hit_count:h.hit_count(),
			active:h.active(),
			condition:h.condition(),
			ignoreCount:h.ignoreCount(),
			actual_locations:h.actual_locations()
		}

		if(h.type()==Debug.ScriptBreakPointType.ScriptId){
			i.type='scriptId';
			i.script_id=h.script_id();
		}else{
			i.type='scriptName';
			i.script_name=h.script_name();
		}
		d.push(i);
	}

	b.body={breakpoints:d}
}


DebugCommandProcessor.prototype.backtraceRequest_=function(a,b){

	var d=this.exec_state_.frameCount();


	if(d==0){
		b.body={
			totalFrames:d
		}
		return;
	}


	var g=0
		var h=kDefaultBacktraceLength;


	if(a.arguments){
		if(a.arguments.fromFrame){
			g=a.arguments.fromFrame;
		}
		if(a.arguments.toFrame){
			h=a.arguments.toFrame;
		}
		if(a.arguments.bottom){
			var i=d-g;
			g=d-h
				h=i;
		}
		if(g<0||h<0){
			return b.failed('Invalid frame number');
		}
	}


	h=Math.min(d,h);

	if(h<=g){
		var j='Invalid frame range';
		return b.failed(j);
	}


	var k=[];
	for(var l=g;l<h;l++){
		k.push(this.exec_state_.frame(l));
	}
	b.body={
		fromFrame:g,
		toFrame:h,
		totalFrames:d,
		frames:k
	}
};


DebugCommandProcessor.prototype.backtracec=function(a,b){
	return this.exec_state_.cframesValue();
};


DebugCommandProcessor.prototype.frameRequest_=function(a,b){

	if(this.exec_state_.frameCount()==0){
		return b.failed('No frames');
	}


	if(a.arguments){
		var d=a.arguments.number;
		if(d<0||this.exec_state_.frameCount()<=d){
			return b.failed('Invalid frame number');
		}

		this.exec_state_.setSelectedFrame(a.arguments.number);
	}
	b.body=this.exec_state_.frame();
};


DebugCommandProcessor.prototype.frameForScopeRequest_=function(a){


	if(a.arguments&&!(typeof(a.arguments.frameNumber)==='undefined')){
		frame_index=a.arguments.frameNumber;
		if(frame_index<0||this.exec_state_.frameCount()<=frame_index){
			return response.failed('Invalid frame number');
		}
		return this.exec_state_.frame(frame_index);
	}else{
		return this.exec_state_.frame();
	}
}


DebugCommandProcessor.prototype.scopesRequest_=function(a,b){

	if(this.exec_state_.frameCount()==0){
		return b.failed('No scopes');
	}


	var d=this.frameForScopeRequest_(a);


	var g=d.scopeCount();
	var h=[];
	for(var i=0;i<g;i++){
		h.push(d.scope(i));
	}
	b.body={
		fromScope:0,
		toScope:g,
		totalScopes:g,
		scopes:h
	}
};


DebugCommandProcessor.prototype.scopeRequest_=function(a,b){

	if(this.exec_state_.frameCount()==0){
		return b.failed('No scopes');
	}


	var d=this.frameForScopeRequest_(a);


	var g=0;
	if(a.arguments&&!(typeof(a.arguments.number)==='undefined')){
		g=%ToNumber(a.arguments.number);
		if(g<0||d.scopeCount()<=g){
			return b.failed('Invalid scope number');
		}
	}

	b.body=d.scope(g);
};


DebugCommandProcessor.prototype.evaluateRequest_=function(a,b){
	if(!a.arguments){
		return b.failed('Missing arguments');
	}


	var d=a.arguments.expression;
	var g=a.arguments.frame;
	var h=a.arguments.global;
	var i=a.arguments.disable_break;



	try{
		d=String(d);
	}catch(e){
		return b.failed('Failed to convert expression argument to string');
	}


	if(!(typeof(g)==='undefined')&&h){
		return b.failed('Arguments "frame" and "global" are exclusive');
	}


	if(h){

		b.body=
			this.exec_state_.evaluateGlobal(d,Boolean(i));
		return;
	}


	if((typeof(i)==='undefined')){
		i=true;
	}


	if(this.exec_state_.frameCount()==0){
		return b.failed('No frames');
	}


	if(!(typeof(g)==='undefined')){
		var j=%ToNumber(g);
		if(j<0||j>=this.exec_state_.frameCount()){
			return b.failed('Invalid frame "'+g+'"');
		}

		b.body=this.exec_state_.frame(j).evaluate(
				d,Boolean(i));
		return;
	}else{

		b.body=this.exec_state_.frame().evaluate(
				d,Boolean(i));
		return;
	}
};


DebugCommandProcessor.prototype.lookupRequest_=function(a,b){
	if(!a.arguments){
		return b.failed('Missing arguments');
	}


	var d=a.arguments.handles;


	if((typeof(d)==='undefined')){
		return b.failed('Argument "handles" missing');
	}


	if(!(typeof(a.arguments.includeSource)==='undefined')){
		includeSource=%ToBoolean(a.arguments.includeSource);
		b.setOption('includeSource',includeSource);
	}


	var g={};
	for(var h=0;h<d.length;h++){
		var i=d[h];
		var j=LookupMirror(i);
		if(!j){
			return b.failed('Object #'+i+'# not found');
		}
		g[i]=j;
	}
	b.body=g;
};


DebugCommandProcessor.prototype.referencesRequest_=
function(a,b){
	if(!a.arguments){
		return b.failed('Missing arguments');
	}


	var d=a.arguments.type;
	var g=a.arguments.handle;


	if((typeof(d)==='undefined')){
		return b.failed('Argument "type" missing');
	}
	if((typeof(g)==='undefined')){
		return b.failed('Argument "handle" missing');
	}
	if(d!='referencedBy'&&d!='constructedBy'){
		return b.failed('Invalid type "'+d+'"');
	}


	var h=LookupMirror(g);
	if(h){
		if(d=='referencedBy'){
			b.body=h.referencedBy();
		}else{
			b.body=h.constructedBy();
		}
	}else{
		return b.failed('Object #'+g+'# not found');
	}
};


DebugCommandProcessor.prototype.sourceRequest_=function(a,b){

	if(this.exec_state_.frameCount()==0){
		return b.failed('No source');
	}

	var d;
	var g;
	var h=this.exec_state_.frame();
	if(a.arguments){

		d=a.arguments.fromLine;
		g=a.arguments.toLine;

		if(!(typeof(a.arguments.frame)==='undefined')){
			var i=%ToNumber(a.arguments.frame);
			if(i<0||i>=this.exec_state_.frameCount()){
				return b.failed('Invalid frame "'+h+'"');
			}
			h=this.exec_state_.frame(i);
		}
	}


	var j=h.func().script();
	if(!j){
		return b.failed('No source');
	}


	var k=j.sourceSlice(d,g);
	if(!k){
		return b.failed('Invalid line interval');
	}
	b.body={};
	b.body.source=k.sourceText();
	b.body.fromLine=k.from_line;
	b.body.toLine=k.to_line;
	b.body.fromPosition=k.from_position;
	b.body.toPosition=k.to_position;
	b.body.totalLines=j.lineCount();
};


DebugCommandProcessor.prototype.scriptsRequest_=function(a,b){
	var d=ScriptTypeFlag(Debug.ScriptType.Normal);
	var g=false;
	var h=null;
	if(a.arguments){

		if(!(typeof(a.arguments.types)==='undefined')){
			d=%ToNumber(a.arguments.types);
			if(isNaN(d)||d<0){
				return b.failed('Invalid types "'+a.arguments.types+'"');
			}
		}

		if(!(typeof(a.arguments.includeSource)==='undefined')){
			g=%ToBoolean(a.arguments.includeSource);
			b.setOption('includeSource',g);
		}

		if((%_IsArray(a.arguments.ids))){
			h={};
			var i=a.arguments.ids;
			for(var j=0;j<i.length;j++){
				h[i[j]]=true;
			}
		}
	}


	var k=%DebugGetLoadedScripts();

	b.body=[];

	for(var j=0;j<k.length;j++){
		if(h&&!h[k[j].id]){
			continue;
		}
		if(d&ScriptTypeFlag(k[j].type)){
			b.body.push(MakeMirror(k[j]));
		}
	}
};


DebugCommandProcessor.prototype.threadsRequest_=function(a,b){

	var d=this.exec_state_.threadCount();


	var g=[];
	for(var h=0;h<d;h++){
		var i=%GetThreadDetails(this.exec_state_.break_id,h);
		var j={current:i[0],
			id:i[1]
		}
		g.push(j);
	}


	b.body={
		totalThreads:d,
		threads:g
	}
};


DebugCommandProcessor.prototype.suspendRequest_=function(a,b){
	b.running=false;
};


DebugCommandProcessor.prototype.versionRequest_=function(a,b){
	b.body={
		V8Version:%GetV8Version()
	}
};


DebugCommandProcessor.prototype.profileRequest_=function(a,b){
	if(!a.arguments){
		return b.failed('Missing arguments');
	}
	var d=parseInt(a.arguments.modules);
	if(isNaN(d)){
		return b.failed('Modules is not an integer');
	}
	var g=parseInt(a.arguments.tag);
	if(isNaN(g)){
		g=0;
	}
	if(a.arguments.command=='resume'){
		%ProfilerResume(d,g);
	}else if(a.arguments.command=='pause'){
		%ProfilerPause(d,g);
	}else{
		return b.failed('Unknown command');
	}
	b.body={};
};


DebugCommandProcessor.prototype.changeLiveRequest_=function(a,b){
	if(!Debug.LiveEdit){
		return b.failed('LiveEdit feature is not supported');
	}
	if(!a.arguments){
		return b.failed('Missing arguments');
	}
	var d=a.arguments.script_id;
	var g=!!a.arguments.preview_only;

	var h=%DebugGetLoadedScripts();

	var i=null;
	for(var j=0;j<h.length;j++){
		if(h[j].id==d){
			i=h[j];
		}
	}
	if(!i){
		b.failed('Script not found');
		return;
	}

	var k=new Array();

	if(!(typeof(a.arguments.new_source)==='string')){
		throw"new_source argument expected";
	}

	var l=a.arguments.new_source;

	var m=Debug.LiveEdit.SetScriptSource(i,
			l,g,k);
	b.body={change_log:k,result:m};

	if(!g&&!this.running_&&m.stack_modified){
		b.body.stepin_recommended=true;
	}
};


DebugCommandProcessor.prototype.debuggerFlagsRequest_=function(request,
		response){

			if(!request.arguments){
				response.failed('Missing arguments');
				return;
			}


			var a=request.arguments.flags;

			response.body={flags:[]};
			if(!(typeof(a)==='undefined')){
				for(var b=0;b<a.length;b++){
					var d=a[b].name;
					var g=debugger_flags[d];
					if(!g){
						continue;
					}
					if('value'in a[b]){
						g.setValue(a[b].value);
					}
					response.body.flags.push({name:d,value:g.getValue()});
				}
			}else{
				for(var d in debugger_flags){
					var h=debugger_flags[d].getValue();
					response.body.flags.push({name:d,value:h});
				}
			}
		}




DebugCommandProcessor.prototype.isRunning=function(){
	return this.running_;
}


DebugCommandProcessor.prototype.systemBreak=function(a,b){
	return %SystemBreak();
};


function NumberToHex8Str(a){
	var b="";
	for(var d=0;d<8;++d){
		var g=hexCharArray[a&0x0F];
		b=g+b;
		a=a>>>4;
	}
	return b;
};

DebugCommandProcessor.prototype.formatCFrames=function(a){
	var b="";
	if(a==null||a.length==0){
		b+="(stack empty)";
	}else{
		for(var d=0;d<a.length;++d){
			if(d!=0)b+="\n";
			b+=this.formatCFrame(a[d]);
		}
	}
	return b;
};


DebugCommandProcessor.prototype.formatCFrame=function(a){
	var b="";
	b+="0x"+NumberToHex8Str(a.address);
	if(!(typeof(a.text)==='undefined')){
		b+=" "+a.text;
	}
	return b;
}














function ObjectToProtocolObject_(a,b){
	var d={};
	for(var g in a){

		if(typeof g=='string'){

			var h=ValueToProtocolValue_(a[g],
					b);

			if(!(typeof(h)==='undefined')){
				d[g]=h;
			}
		}
	}

	return d;
}










function ArrayToProtocolArray_(a,b){
	var d=[];
	for(var g=0;g<a.length;g++){
		d.push(ValueToProtocolValue_(a[g],b));
	}
	return d;
}









function ValueToProtocolValue_(a,b){

	var d;
	switch(typeof a){
		case'object':
			if(a instanceof Mirror){
				d=b.serializeValue(a);
			}else if((%_IsArray(a))){
				d=ArrayToProtocolArray_(a,b);
			}else{
				d=ObjectToProtocolObject_(a,b);
			}
			break;

		case'boolean':
		case'string':
		case'number':
			d=a;
			break

		default:
				d=null;
	}
	return d;
}













































Debug.LiveEdit=new function(){


	var a;





	function ApplyPatchMultiChunk(script,diff_array,new_source,preview_only,
			change_log){

				var b=script.source;


				var d=GatherCompileInfo(b,script);


				var g=BuildCodeInfoTree(d);

				var h=new PosTranslator(diff_array);


				MarkChangedFunctions(g,h.GetChunks());


				FindLiveSharedInfos(g,script);


				var i;
				try{
					i=GatherCompileInfo(new_source,script);
				}catch(e){
					throw new Failure("Failed to compile new version of script: "+e);
				}
				var j=BuildCodeInfoTree(i);


				FindCorrespondingFunctions(g,j);


				var k=new Array();
				var l=new Array();
				var m=new Array();
				var o=new Array();

				function HarvestTodo(q){
					function CollectDamaged(r){
						l.push(r);
						for(var t=0;t<r.children.length;t++){
							CollectDamaged(r.children[t]);
						}
					}



					function CollectNew(u){
						for(var t=0;t<u.length;t++){
							m.push(u[t]);
							CollectNew(u[t].children);
						}
					}

					if(q.status==a.DAMAGED){
						CollectDamaged(q);
						return;
					}
					if(q.status==a.UNCHANGED){
						o.push(q);
					}else if(q.status==a.SOURCE_CHANGED){
						o.push(q);
					}else if(q.status==a.CHANGED){
						k.push(q);
						CollectNew(q.unmatched_new_nodes);
					}
					for(var t=0;t<q.children.length;t++){
						HarvestTodo(q.children[t]);
					}
				}

				var v={
					change_tree:DescribeChangeTree(g),
					textual_diff:{
						old_len:b.length,
						new_len:new_source.length,
						chunks:diff_array
					},
					updated:false
				};

				if(preview_only){
					return v;
				}

				HarvestTodo(g);


				var w=new Array();
				for(var t=0;t<k.length;t++){
					var z=k[t].live_shared_info_wrapper;
					if(z){
						w.push(z);
					}
				}





				var A=
					CheckStackActivations(w,change_log);

				v.stack_modified=A!=0;



				var B=TemporaryRemoveBreakPoints(script,change_log);

				var C;



				if(l.length==0){
					%LiveEditReplaceScript(script,new_source,null);
					C=void 0;
				}else{
					var D=CreateNameForOldScript(script);



					C=%LiveEditReplaceScript(script,new_source,
							D);

					var E=new Array();
					change_log.push({linked_to_old_script:E});


					for(var t=0;t<l.length;t++){
						LinkToOldScript(l[t],C,
								E);
					}

					v.created_script_name=D;
				}


				for(var t=0;t<m.length;t++){
					%LiveEditFunctionSetScript(
							m[t].info.shared_function_info,script);
				}

				for(var t=0;t<k.length;t++){
					PatchFunctionCode(k[t],change_log);
				}

				var G=new Array();
				change_log.push({position_patched:G});

				for(var t=0;t<o.length;t++){


					PatchPositions(o[t],diff_array,
							G);
				}

				B(h,C);

				v.updated=true;
				return v;
			}

	this.ApplyPatchMultiChunk=ApplyPatchMultiChunk;











	function GatherCompileInfo(H,I){


		var J=%LiveEditGatherCompileInfo(I,H);


		var K=new Array();
		var L=new Array();
		for(var t=0;t<J.length;t++){
			var M=new FunctionCompileInfo(J[t]);




			%LiveEditFunctionSetScript(M.shared_function_info,void 0);
			K.push(M);
			L.push(t);
		}

		for(var t=0;t<K.length;t++){
			var N=t;
			for(var O=t+1;O<K.length;O++){
				if(K[N].start_position>K[O].start_position){
					N=O;
				}
			}
			if(N!=t){
				var P=K[N];
				var Q=L[N];
				K[N]=K[t];
				L[N]=L[t];
				K[t]=P;
				L[t]=Q;
			}
		}



		var R=0;



		function ResetIndexes(S,T){
			var U=-1;
			while(R<K.length&&
					K[R].outer_index==T){
						var W=R;
						K[W].outer_index=S;
						if(U!=-1){
							K[U].next_sibling_index=W;
						}
						U=W;
						R++;
						ResetIndexes(W,L[W]);
					}
			if(U!=-1){
				K[U].next_sibling_index=-1;
			}
		}

		ResetIndexes(-1,-1);
		Assert(R==K.length);

		return K;
	}



	function PatchFunctionCode(q,X){
		var Y=q.corresponding_node.info;
		var Z=q.live_shared_info_wrapper;
		if(Z){
			%LiveEditReplaceFunctionCode(Y.raw_array,
					Z.raw_array);







			for(var t=0;t<q.children.length;t++){
				if(q.children[t].corresponding_node){
					var aa=q.children[t].corresponding_node;
					var ab=
						q.children[t].live_shared_info_wrapper;
					if(ab){
						%LiveEditReplaceRefToNestedFunction(Z.info,
								aa.info.shared_function_info,
								ab.info);
					}
				}
			}

			X.push({function_patched:Y.function_name});
		}else{
			X.push({function_patched:Y.function_name,
				function_info_not_found:true});
		}
	}





	function LinkToOldScript(ac,C,ad){
		var ae=ac.live_shared_info_wrapper;
		if(ae){
			%LiveEditFunctionSetScript(ae.info,C);
			ad.push({name:ae.function_name});
		}else{
			ad.push(
					{name:ac.info.function_name,not_found:true});
		}
	}



	function TemporaryRemoveBreakPoints(af,X){
		var ag=GetScriptBreakPoints(af);

		var ah=[];
		X.push({break_points_update:ah});

		var ai=[];
		for(var t=0;t<ag.length;t++){
			var aj=ag[t];

			aj.clear();


			var ak=Debug.findScriptSourcePosition(af,
					aj.line(),aj.column());

			var al={
				position:ak,
				line:aj.line(),
				column:aj.column()
			}
			ai.push(al);
		}




		return function(h,am){


			for(var t=0;t<ag.length;t++){
				var aj=ag[t];
				if(am){
					var an=aj.cloneForOtherScript(am);
					an.set(am);

					ah.push({
						type:"copied_to_old",
						id:aj.number(),
						new_id:an.number(),
						positions:ai[t]
					});
				}

				var ao=h.Translate(
						ai[t].position,
						PosTranslator.ShiftWithTopInsideChunkHandler);

				var ap=
					af.locationFromPosition(ao,false);

				aj.update_positions(ap.line,ap.column);

				var aq={
					position:ao,
					line:ap.line,
					column:ap.column
				}

				aj.set(af);

				ah.push({type:"position_changed",
					id:aj.number(),
					old_positions:ai[t],
					new_positions:aq
				});
			}
		}
	}


	function Assert(ar,as){
		if(!ar){
			if(as){
				throw"Assert "+as;
			}else{
				throw"Assert";
			}
		}
	}

	function DiffChunk(at,au,av,aw){
		this.pos1=at;
		this.pos2=au;
		this.len1=av;
		this.len2=aw;
	}

	function PosTranslator(ax){
		var ay=new Array();
		var az=0;
		for(var t=0;t<ax.length;t+=3){
			var aA=ax[t];
			var aB=aA+az;
			var aC=ax[t+1];
			var aD=ax[t+2];
			ay.push(new DiffChunk(aA,aB,aC-aA,
						aD-aB));
			az=aD-aC;
		}
		this.chunks=ay;
	}
	PosTranslator.prototype.GetChunks=function(){
		return this.chunks;
	}

	PosTranslator.prototype.Translate=function(aE,aF){
		var aG=this.chunks;
		if(aG.length==0||aE<aG[0].pos1){
			return aE;
		}
		var aH=0;
		var aI=aG.length-1;

		while(aH<aI){
			var aJ=Math.floor((aH+aI)/2);
			if(aE<aG[aJ+1].pos1){
				aI=aJ;
			}else{
				aH=aJ+1;
			}
		}
		var aK=aG[aH];
		if(aE>=aK.pos1+aK.len1){
			return aE+aK.pos2+aK.len2-aK.pos1-aK.len1;
		}

		if(!aF){
			aF=PosTranslator.DefaultInsideChunkHandler;
		}
		return aF(aE,aK);
	}

	PosTranslator.DefaultInsideChunkHandler=function(aE,aL){
		Assert(false,"Cannot translate position in changed area");
	}

	PosTranslator.ShiftWithTopInsideChunkHandler=
		function(aE,aL){


			return aE-aL.pos1+aL.pos2;
		}

	var a={


		UNCHANGED:"unchanged",


		SOURCE_CHANGED:"source changed",


		CHANGED:"changed",

		DAMAGED:"damaged"
	}

	function CodeInfoTreeNode(aM,aN,aO){
		this.info=aM;
		this.children=aN;

		this.array_index=aO;
		this.parent=void 0;

		this.status=a.UNCHANGED;


		this.status_explanation=void 0;
		this.new_start_pos=void 0;
		this.new_end_pos=void 0;
		this.corresponding_node=void 0;
		this.unmatched_new_nodes=void 0;







		this.textual_corresponding_node=void 0;
		this.textually_unmatched_new_nodes=void 0;

		this.live_shared_info_wrapper=void 0;
	}



	function BuildCodeInfoTree(aP){

		var aQ=0;


		function BuildNode(){
			var aR=aQ;
			aQ++;
			var aS=new Array();
			while(aQ<aP.length&&
					aP[aQ].outer_index==aR){
						aS.push(BuildNode());
					}
			var r=new CodeInfoTreeNode(aP[aR],aS,
					aR);
			for(var t=0;t<aS.length;t++){
				aS[t].parent=r;
			}
			return r;
		}

		var aT=BuildNode();
		Assert(aQ==aP.length);
		return aT;
	}




	function MarkChangedFunctions(aU,ay){



		var aV=new function(){
			var aW=0;
			var aX=0;
			this.current=function(){return ay[aW];}
			this.next=function(){
				var aK=ay[aW];
				aX=aK.pos2+aK.len2-(aK.pos1+aK.len1);
				aW++;
			}
			this.done=function(){return aW>=ay.length;}
			this.TranslatePos=function(aE){return aE+aX;}
		};




		function ProcessInternals(aY){
			aY.new_start_pos=aV.TranslatePos(
					aY.info.start_position);
			var aZ=0;
			var ba=false;
			var bb=false;

			while(!aV.done()&&
					aV.current().pos1<aY.info.end_position){
						if(aZ<aY.children.length){
							var bc=aY.children[aZ];

							if(bc.info.end_position<=aV.current().pos1){
								ProcessUnchangedChild(bc);
								aZ++;
								continue;
							}else if(bc.info.start_position>=
									aV.current().pos1+aV.current().len1){
										ba=true;
										aV.next();
										continue;
									}else if(bc.info.start_position<=aV.current().pos1&&
											bc.info.end_position>=aV.current().pos1+
											aV.current().len1){
												ProcessInternals(bc);
												bb=bb||
													(bc.status!=a.UNCHANGED);
												ba=ba||
													(bc.status==a.DAMAGED);
												aZ++;
												continue;
											}else{
												ba=true;
												bc.status=a.DAMAGED;
												bc.status_explanation=
													"Text diff overlaps with function boundary";
												aZ++;
												continue;
											}
						}else{
							if(aV.current().pos1+aV.current().len1<=
									aY.info.end_position){
										aY.status=a.CHANGED;
										aV.next();
										continue;
									}else{
										aY.status=a.DAMAGED;
										aY.status_explanation=
											"Text diff overlaps with function boundary";
										return;
									}
						}
						Assert("Unreachable",false);
					}
			while(aZ<aY.children.length){
				var bc=aY.children[aZ];
				ProcessUnchangedChild(bc);
				aZ++;
			}
			if(ba){
				aY.status=a.CHANGED;
			}else if(bb){
				aY.status=a.SOURCE_CHANGED;
			}
			aY.new_end_pos=
				aV.TranslatePos(aY.info.end_position);
		}

		function ProcessUnchangedChild(r){
			r.new_start_pos=aV.TranslatePos(r.info.start_position);
			r.new_end_pos=aV.TranslatePos(r.info.end_position);
		}

		ProcessInternals(aU);
	}








	function FindCorrespondingFunctions(bd,be){



		function ProcessChildren(q,bf){
			var bg=q.children;
			var bh=bf.children;

			var bi=[];
			var bj=[];

			var bk=0;
			var bl=0;
			while(bk<bg.length){
				if(bg[bk].status==a.DAMAGED){
					bk++;
				}else if(bl<bh.length){
					if(bh[bl].info.start_position<
							bg[bk].new_start_pos){
								bi.push(bh[bl]);
								bj.push(bh[bl]);
								bl++;
							}else if(bh[bl].info.start_position==
									bg[bk].new_start_pos){
										if(bh[bl].info.end_position==
												bg[bk].new_end_pos){
													bg[bk].corresponding_node=
														bh[bl];
													bg[bk].textual_corresponding_node=
														bh[bl];
													if(bg[bk].status!=a.UNCHANGED){
														ProcessChildren(bg[bk],
																bh[bl]);
														if(bg[bk].status==a.DAMAGED){
															bi.push(
																	bg[bk].corresponding_node);
															bg[bk].corresponding_node=void 0;
															q.status=a.CHANGED;
														}
													}
												}else{
													bg[bk].status=a.DAMAGED;
													bg[bk].status_explanation=
														"No corresponding function in new script found";
													q.status=a.CHANGED;
													bi.push(bh[bl]);
													bj.push(bh[bl]);
												}
										bl++;
										bk++;
									}else{
										bg[bk].status=a.DAMAGED;
										bg[bk].status_explanation=
											"No corresponding function in new script found";
										q.status=a.CHANGED;
										bk++;
									}
				}else{
					bg[bk].status=a.DAMAGED;
					bg[bk].status_explanation=
						"No corresponding function in new script found";
					q.status=a.CHANGED;
					bk++;
				}
			}

			while(bl<bh.length){
				bi.push(bh[bl]);
				bj.push(bh[bl]);
				bl++;
			}

			if(q.status==a.CHANGED){
				var bm=
					WhyFunctionExpectationsDiffer(q.info,bf.info);
				if(bm){
					q.status=a.DAMAGED;
					q.status_explanation=bm;
				}
			}
			q.unmatched_new_nodes=bi;
			q.textually_unmatched_new_nodes=
				bj;
		}

		ProcessChildren(bd,be);

		bd.corresponding_node=be;
		bd.textual_corresponding_node=be;

		Assert(bd.status!=a.DAMAGED,
				"Script became damaged");
	}

	function FindLiveSharedInfos(bd,I){
		var bn=%LiveEditFindSharedFunctionInfosForScript(I);

		var bo=new Array();

		for(var t=0;t<bn.length;t++){
			bo.push(new SharedInfoWrapper(bn[t]));
		}



		function FindFunctionInfo(K){
			for(var t=0;t<bo.length;t++){
				var bp=bo[t];
				if(bp.start_position==K.start_position&&
						bp.end_position==K.end_position){
							return bp;
						}
			}
		}

		function TraverseTree(r){
			var z=FindFunctionInfo(r.info);
			if(z){
				r.live_shared_info_wrapper=z;
			}
			for(var t=0;t<r.children.length;t++){
				TraverseTree(r.children[t]);
			}
		}

		TraverseTree(bd);
	}




	function FunctionCompileInfo(bq){
		this.function_name=bq[0];
		this.start_position=bq[1];
		this.end_position=bq[2];
		this.param_num=bq[3];
		this.code=bq[4];
		this.code_scope_info=bq[5];
		this.scope_info=bq[6];
		this.outer_index=bq[7];
		this.shared_function_info=bq[8];
		this.next_sibling_index=null;
		this.raw_array=bq;
	}

	function SharedInfoWrapper(bq){
		this.function_name=bq[0];
		this.start_position=bq[1];
		this.end_position=bq[2];
		this.info=bq[3];
		this.raw_array=bq;
	}


	function PatchPositions(ac,ax,ad){
		var Z=ac.live_shared_info_wrapper;
		if(!Z){

			ad.push(
					{name:ac.info.function_name,info_not_found:true});
			return;
		}
		%LiveEditPatchFunctionPositions(Z.raw_array,
				ax);
		ad.push({name:ac.info.function_name});
	}


	function CreateNameForOldScript(I){

		return I.name+" (old)";
	}



	function WhyFunctionExpectationsDiffer(br,bs){


		if(br.param_num!=bs.param_num){
			return"Changed parameter number: "+br.param_num+
				" and "+bs.param_num;
		}
		var bt=br.scope_info;
		var bu=bs.scope_info;

		var bv;
		var bw;

		if(bt){
			bv=bt.toString();
		}else{
			bv="";
		}
		if(bu){
			bw=bu.toString();
		}else{
			bw="";
		}

		if(bv!=bw){
			return"Incompatible variable maps: ["+bv+
				"] and ["+bw+"]";
		}

		return;
	}


	var bx;




	function CheckStackActivations(by,X){
		var bz=new Array();
		for(var t=0;t<by.length;t++){
			bz[t]=by[t].info;
		}
		var bA=%LiveEditCheckAndDropActivations(bz,true);
		if(bA[bz.length]){

			throw new Failure(bA[bz.length]);
		}

		var bB=new Array();
		var bC=new Array();
		for(var t=0;t<bz.length;t++){
			var bD=by[t];
			if(bA[t]==bx.REPLACED_ON_ACTIVE_STACK){
				bC.push({name:bD.function_name});
			}else if(bA[t]!=bx.AVAILABLE_FOR_PATCH){
				var bE={
					name:bD.function_name,
					start_pos:bD.start_position,
					end_pos:bD.end_position,
					replace_problem:
						bx.SymbolName(bA[t])
				};
				bB.push(bE);
			}
		}
		if(bC.length>0){
			X.push({dropped_from_stack:bC});
		}
		if(bB.length>0){
			X.push({functions_on_stack:bB});
			throw new Failure("Blocked by functions on stack");
		}

		return bC.length;
	}


	var bx={
		AVAILABLE_FOR_PATCH:1,
		BLOCKED_ON_ACTIVE_STACK:2,
		BLOCKED_ON_OTHER_STACK:3,
		BLOCKED_UNDER_NATIVE_CODE:4,
		REPLACED_ON_ACTIVE_STACK:5
	}

	bx.SymbolName=function(bF){
		var bG=bx;
		for(name in bG){
			if(bG[name]==bF){
				return name;
			}
		}
	}




	function Failure(as){
		this.message=as;
	}

	this.Failure=Failure;

	Failure.prototype.toString=function(){
		return"LiveEdit Failure: "+this.message;
	}


	function GetPcFromSourcePos(bH,bI){
		return %GetFunctionCodePositionFromSource(bH,bI);
	}

	this.GetPcFromSourcePos=GetPcFromSourcePos;


	function SetScriptSource(I,bJ,bK,X){
		var b=I.source;
		var bL=CompareStringsLinewise(b,bJ);
		return ApplyPatchMultiChunk(I,bL,bJ,bK,
				X);
	}

	this.SetScriptSource=SetScriptSource;

	function CompareStringsLinewise(bM,bN){
		return %LiveEditCompareStringsLinewise(bM,bN);
	}









	function ApplySingleChunkPatch(I,change_pos,change_len,new_str,
			X){
				var b=I.source;


				var bJ=b.substring(0,change_pos)+
					new_str+b.substring(change_pos+change_len);

				return ApplyPatchMultiChunk(I,
						[change_pos,change_pos+change_len,change_pos+new_str.length],
						bJ,false,X);
			}


	function DescribeChangeTree(bd){

		function ProcessOldNode(r){
			var bO=[];
			for(var t=0;t<r.children.length;t++){
				var bc=r.children[t];
				if(bc.status!=a.UNCHANGED){
					bO.push(ProcessOldNode(bc));
				}
			}
			var bP=[];
			if(r.textually_unmatched_new_nodes){
				for(var t=0;t<r.textually_unmatched_new_nodes.length;t++){
					var bc=r.textually_unmatched_new_nodes[t];
					bP.push(ProcessNewNode(bc));
				}
			}
			var bQ={
				name:r.info.function_name,
				positions:DescribePositions(r),
				status:r.status,
				children:bO,
				new_children:bP
			};
			if(r.status_explanation){
				bQ.status_explanation=r.status_explanation;
			}
			if(r.textual_corresponding_node){
				bQ.new_positions=DescribePositions(r.textual_corresponding_node);
			}
			return bQ;
		}

		function ProcessNewNode(r){
			var bO=[];

			if(false){
				for(var t=0;t<r.children.length;t++){
					bO.push(ProcessNewNode(r.children[t]));
				}
			}
			var bQ={
				name:r.info.function_name,
				positions:DescribePositions(r),
				children:bO,
			};
			return bQ;
		}

		function DescribePositions(r){
			return{
				start_position:r.info.start_position,
				end_position:r.info.end_position
			};
		}

		return ProcessOldNode(bd);
	}



	this.TestApi={
		PosTranslator:PosTranslator,
		CompareStringsLinewise:CompareStringsLinewise,
		ApplySingleChunkPatch:ApplySingleChunkPatch
	}
}





























var next_handle_=0;
var next_transient_handle_=-1;


var mirror_cache_=[];





function ClearMirrorCache(){
	next_handle_=0;
	mirror_cache_=[];
}










function MakeMirror(a,b){
	var d;


	if(!b){
		for(id in mirror_cache_){
			d=mirror_cache_[id];
			if(d.value()===a){
				return d;
			}

			if(d.isNumber()&&isNaN(d.value())&&
					typeof a=='number'&&isNaN(a)){
						return d;
					}
		}
	}

	if((typeof(a)==='undefined')){
		d=new UndefinedMirror();
	}else if((a===null)){
		d=new NullMirror();
	}else if((typeof(a)==='boolean')){
		d=new BooleanMirror(a);
	}else if((typeof(a)==='number')){
		d=new NumberMirror(a);
	}else if((typeof(a)==='string')){
		d=new StringMirror(a);
	}else if((%_IsArray(a))){
		d=new ArrayMirror(a);
	}else if((%_ClassOf(a)==='Date')){
		d=new DateMirror(a);
	}else if((%_IsFunction(a))){
		d=new FunctionMirror(a);
	}else if((%_IsRegExp(a))){
		d=new RegExpMirror(a);
	}else if((%_ClassOf(a)==='Error')){
		d=new ErrorMirror(a);
	}else if((%_ClassOf(a)==='Script')){
		d=new ScriptMirror(a);
	}else{
		d=new ObjectMirror(a,OBJECT_TYPE,b);
	}

	mirror_cache_[d.handle()]=d;
	return d;
}









function LookupMirror(a){
	return mirror_cache_[a];
}







function GetUndefinedMirror(){
	return MakeMirror(void 0);
}















function inherits(a,b){
	var d=function(){};
	d.prototype=b.prototype;
	a.super_=b.prototype;
	a.prototype=new d();
	a.prototype.constructor=a;
}



const UNDEFINED_TYPE='undefined';
const NULL_TYPE='null';
const BOOLEAN_TYPE='boolean';
const NUMBER_TYPE='number';
const STRING_TYPE='string';
const OBJECT_TYPE='object';
const FUNCTION_TYPE='function';
const REGEXP_TYPE='regexp';
const ERROR_TYPE='error';
const PROPERTY_TYPE='property';
const FRAME_TYPE='frame';
const SCRIPT_TYPE='script';
const CONTEXT_TYPE='context';
const SCOPE_TYPE='scope';


const kMaxProtocolStringLength=80;


PropertyKind={};
PropertyKind.Named=1;
PropertyKind.Indexed=2;



PropertyType={};
PropertyType.Normal=0;
PropertyType.Field=1;
PropertyType.ConstantFunction=2;
PropertyType.Callbacks=3;
PropertyType.Interceptor=4;
PropertyType.MapTransition=5;
PropertyType.ConstantTransition=6;
PropertyType.NullDescriptor=7;



PropertyAttribute={};
PropertyAttribute.None=0;
PropertyAttribute.ReadOnly=1;
PropertyAttribute.DontEnum=2;
PropertyAttribute.DontDelete=4;



ScopeType={Global:0,
	Local:1,
	With:2,
	Closure:3,
	Catch:4};


























function Mirror(a){
	this.type_=a;
};


Mirror.prototype.type=function(){
	return this.type_;
};






Mirror.prototype.isValue=function(){
	return this instanceof ValueMirror;
}






Mirror.prototype.isUndefined=function(){
	return this instanceof UndefinedMirror;
}






Mirror.prototype.isNull=function(){
	return this instanceof NullMirror;
}






Mirror.prototype.isBoolean=function(){
	return this instanceof BooleanMirror;
}






Mirror.prototype.isNumber=function(){
	return this instanceof NumberMirror;
}






Mirror.prototype.isString=function(){
	return this instanceof StringMirror;
}






Mirror.prototype.isObject=function(){
	return this instanceof ObjectMirror;
}






Mirror.prototype.isFunction=function(){
	return this instanceof FunctionMirror;
}






Mirror.prototype.isUnresolvedFunction=function(){
	return this instanceof UnresolvedFunctionMirror;
}






Mirror.prototype.isArray=function(){
	return this instanceof ArrayMirror;
}






Mirror.prototype.isDate=function(){
	return this instanceof DateMirror;
}






Mirror.prototype.isRegExp=function(){
	return this instanceof RegExpMirror;
}






Mirror.prototype.isError=function(){
	return this instanceof ErrorMirror;
}






Mirror.prototype.isProperty=function(){
	return this instanceof PropertyMirror;
}






Mirror.prototype.isFrame=function(){
	return this instanceof FrameMirror;
}






Mirror.prototype.isScript=function(){
	return this instanceof ScriptMirror;
}






Mirror.prototype.isContext=function(){
	return this instanceof ContextMirror;
}






Mirror.prototype.isScope=function(){
	return this instanceof ScopeMirror;
}





Mirror.prototype.allocateHandle_=function(){
	this.handle_=next_handle_++;
}






Mirror.prototype.allocateTransientHandle_=function(){
	this.handle_=next_transient_handle_--;
}


Mirror.prototype.toText=function(){

	return"#<"+builtins.GetInstanceName(this.constructor.name)+">";
}











function ValueMirror(a,b,d){
	Mirror.call(this,a);
	this.value_=b;
	if(!d){
		this.allocateHandle_();
	}else{
		this.allocateTransientHandle_();
	}
}
inherits(ValueMirror,Mirror);


Mirror.prototype.handle=function(){
	return this.handle_;
};






ValueMirror.prototype.isPrimitive=function(){
	var a=this.type();
	return a==='undefined'||
		a==='null'||
		a==='boolean'||
		a==='number'||
		a==='string';
};






ValueMirror.prototype.value=function(){
	return this.value_;
};







function UndefinedMirror(){
	ValueMirror.call(this,UNDEFINED_TYPE,void 0);
}
inherits(UndefinedMirror,ValueMirror);


UndefinedMirror.prototype.toText=function(){
	return'undefined';
}







function NullMirror(){
	ValueMirror.call(this,NULL_TYPE,null);
}
inherits(NullMirror,ValueMirror);


NullMirror.prototype.toText=function(){
	return'null';
}








function BooleanMirror(a){
	ValueMirror.call(this,BOOLEAN_TYPE,a);
}
inherits(BooleanMirror,ValueMirror);


BooleanMirror.prototype.toText=function(){
	return this.value_?'true':'false';
}








function NumberMirror(a){
	ValueMirror.call(this,NUMBER_TYPE,a);
}
inherits(NumberMirror,ValueMirror);


NumberMirror.prototype.toText=function(){
	return %NumberToString(this.value_);
}








function StringMirror(a){
	ValueMirror.call(this,STRING_TYPE,a);
}
inherits(StringMirror,ValueMirror);


StringMirror.prototype.length=function(){
	return this.value_.length;
};

StringMirror.prototype.getTruncatedValue=function(a){
	if(a!=-1&&this.length()>a){
		return this.value_.substring(0,a)+
			'... (length: '+this.length()+')';
					}
					return this.value_;
					}

					StringMirror.prototype.toText=function(){
						return this.getTruncatedValue(kMaxProtocolStringLength);
					}










					function ObjectMirror(a,b,d){
						ValueMirror.call(this,b||OBJECT_TYPE,a,d);
					}
inherits(ObjectMirror,ValueMirror);


ObjectMirror.prototype.className=function(){
	return %_ClassOf(this.value_);
};


ObjectMirror.prototype.constructorFunction=function(){
	return MakeMirror(%DebugGetProperty(this.value_,'constructor'));
};


ObjectMirror.prototype.prototypeObject=function(){
	return MakeMirror(%DebugGetProperty(this.value_,'prototype'));
};


ObjectMirror.prototype.protoObject=function(){
	return MakeMirror(%DebugGetPrototype(this.value_));
};


ObjectMirror.prototype.hasNamedInterceptor=function(){

	var a=%GetInterceptorInfo(this.value_);
	return(a&2)!=0;
};


ObjectMirror.prototype.hasIndexedInterceptor=function(){

	var a=%GetInterceptorInfo(this.value_);
	return(a&1)!=0;
};










ObjectMirror.prototype.propertyNames=function(a,b){

	a=a||PropertyKind.Named|PropertyKind.Indexed;

	var d;
	var g;
	var h=0;


	if(a&PropertyKind.Named){

		d=%GetLocalPropertyNames(this.value_);
		h+=d.length;


		if(this.hasNamedInterceptor()&&(a&PropertyKind.Named)){
			var i=
				%GetNamedInterceptorPropertyNames(this.value_);
			if(i){
				d=d.concat(i);
				h+=i.length;
			}
		}
	}


	if(a&PropertyKind.Indexed){

		g=%GetLocalElementNames(this.value_);
		h+=g.length;


		if(this.hasIndexedInterceptor()&&(a&PropertyKind.Indexed)){
			var j=
				%GetIndexedInterceptorElementNames(this.value_);
			if(j){
				g=g.concat(j);
				h+=j.length;
			}
		}
	}
	b=Math.min(b||h,h);

	var k=new Array(b);
	var l=0;


	if(a&PropertyKind.Named){
		for(var m=0;l<b&&m<d.length;m++){
			k[l++]=d[m];
		}
	}


	if(a&PropertyKind.Indexed){
		for(var m=0;l<b&&m<g.length;m++){
			k[l++]=g[m];
		}
	}

	return k;
};










ObjectMirror.prototype.properties=function(a,b){
	var d=this.propertyNames(a,b);
	var g=new Array(d.length);
	for(var h=0;h<d.length;h++){
		g[h]=this.property(d[h]);
	}

	return g;
};


ObjectMirror.prototype.property=function(a){
	var b=%DebugGetPropertyDetails(this.value_,%ToString(a));
	if(b){
		return new PropertyMirror(this,a,b);
	}


	return GetUndefinedMirror();
};









ObjectMirror.prototype.lookupProperty=function(a){
	var b=this.properties();


	for(var d=0;d<b.length;d++){


		var g=b[d];
		if(g.propertyType()!=PropertyType.Callbacks){
			if(%_ObjectEquals(g.value_,a.value_)){
				return g;
			}
		}
	}


	return GetUndefinedMirror();
};








ObjectMirror.prototype.referencedBy=function(a){

	var b=%DebugReferencedBy(this.value_,
			Mirror.prototype,a||0);


	for(var d=0;d<b.length;d++){
		b[d]=MakeMirror(b[d]);
	}

	return b;
};


ObjectMirror.prototype.toText=function(){
	var a;
	var b=this.constructorFunction();
	if(!b.isFunction()){
		a=this.className();
	}else{
		a=b.name();
		if(!a){
			a=this.className();
		}
	}
	return'#<'+builtins.GetInstanceName(a)+'>';
};








function FunctionMirror(a){
	ObjectMirror.call(this,a,FUNCTION_TYPE);
	this.resolved_=true;
}
inherits(FunctionMirror,ObjectMirror);







FunctionMirror.prototype.resolved=function(){
	return this.resolved_;
};






FunctionMirror.prototype.name=function(){
	return %FunctionGetName(this.value_);
};






FunctionMirror.prototype.inferredName=function(){
	return %FunctionGetInferredName(this.value_);
};







FunctionMirror.prototype.source=function(){


	if(this.resolved()){
		return builtins.FunctionSourceString(this.value_);
	}
};







FunctionMirror.prototype.script=function(){


	if(this.resolved()){
		var a=%FunctionGetScript(this.value_);
		if(a){
			return MakeMirror(a);
		}
	}
};







FunctionMirror.prototype.sourcePosition_=function(){


	if(this.resolved()){
		return %FunctionGetScriptSourcePosition(this.value_);
	}
};







FunctionMirror.prototype.sourceLocation=function(){
	if(this.resolved()&&this.script()){
		return this.script().locationFromPosition(this.sourcePosition_(),
				true);
	}
};








FunctionMirror.prototype.constructedBy=function(a){
	if(this.resolved()){

		var b=%DebugConstructedBy(this.value_,a||0);


		for(var d=0;d<b.length;d++){
			b[d]=MakeMirror(b[d]);
		}

		return b;
	}else{
		return[];
	}
};


FunctionMirror.prototype.toText=function(){
	return this.source();
}









function UnresolvedFunctionMirror(a){


	ValueMirror.call(this,FUNCTION_TYPE,a);
	this.propertyCount_=0;
	this.elementCount_=0;
	this.resolved_=false;
}
inherits(UnresolvedFunctionMirror,FunctionMirror);


UnresolvedFunctionMirror.prototype.className=function(){
	return'Function';
};


UnresolvedFunctionMirror.prototype.constructorFunction=function(){
	return GetUndefinedMirror();
};


UnresolvedFunctionMirror.prototype.prototypeObject=function(){
	return GetUndefinedMirror();
};


UnresolvedFunctionMirror.prototype.protoObject=function(){
	return GetUndefinedMirror();
};


UnresolvedFunctionMirror.prototype.name=function(){
	return this.value_;
};


UnresolvedFunctionMirror.prototype.inferredName=function(){
	return undefined;
};


UnresolvedFunctionMirror.prototype.propertyNames=function(a,b){
	return[];
}








function ArrayMirror(a){
	ObjectMirror.call(this,a);
}
inherits(ArrayMirror,ObjectMirror);


ArrayMirror.prototype.length=function(){
	return this.value_.length;
};


ArrayMirror.prototype.indexedPropertiesFromRange=function(a,b){
	var d=a||0;
	var g=b||this.length()-1;
	if(d>g)return new Array();
	var h=new Array(g-d+1);
	for(var i=d;i<=g;i++){
		var j=%DebugGetPropertyDetails(this.value_,%ToString(i));
		var k;
		if(j){
			k=new PropertyMirror(this,i,j);
		}else{
			k=GetUndefinedMirror();
		}
		h[i-d]=k;
	}
	return h;
}








function DateMirror(a){
	ObjectMirror.call(this,a);
}
inherits(DateMirror,ObjectMirror);


DateMirror.prototype.toText=function(){
	var a=JSON.stringify(this.value_);
	return a.substring(1,a.length-1);
}








function RegExpMirror(a){
	ObjectMirror.call(this,a,REGEXP_TYPE);
}
inherits(RegExpMirror,ObjectMirror);






RegExpMirror.prototype.source=function(){
	return this.value_.source;
};






RegExpMirror.prototype.global=function(){
	return this.value_.global;
};






RegExpMirror.prototype.ignoreCase=function(){
	return this.value_.ignoreCase;
};






RegExpMirror.prototype.multiline=function(){
	return this.value_.multiline;
};


RegExpMirror.prototype.toText=function(){

	return"/"+this.source()+"/";
}








function ErrorMirror(a){
	ObjectMirror.call(this,a,ERROR_TYPE);
}
inherits(ErrorMirror,ObjectMirror);






ErrorMirror.prototype.message=function(){
	return this.value_.message;
};


ErrorMirror.prototype.toText=function(){

	var a;
	try{
		str=builtins.ToDetailString(this.value_);
	}catch(e){
		str='#<an Error>';
	}
	return str;
}










function PropertyMirror(a,b,d){
	Mirror.call(this,PROPERTY_TYPE);
	this.mirror_=a;
	this.name_=b;
	this.value_=d[0];
	this.details_=d[1];
	if(d.length>2){
		this.exception_=d[2]
			this.getter_=d[3];
		this.setter_=d[4];
	}
}
inherits(PropertyMirror,Mirror);


PropertyMirror.prototype.isReadOnly=function(){
	return(this.attributes()&PropertyAttribute.ReadOnly)!=0;
}


PropertyMirror.prototype.isEnum=function(){
	return(this.attributes()&PropertyAttribute.DontEnum)==0;
}


PropertyMirror.prototype.canDelete=function(){
	return(this.attributes()&PropertyAttribute.DontDelete)==0;
}


PropertyMirror.prototype.name=function(){
	return this.name_;
}


PropertyMirror.prototype.isIndexed=function(){
	for(var a=0;a<this.name_.length;a++){
		if(this.name_[a]<'0'||'9'<this.name_[a]){
			return false;
		}
	}
	return true;
}


PropertyMirror.prototype.value=function(){
	return MakeMirror(this.value_,false);
}






PropertyMirror.prototype.isException=function(){
	return this.exception_?true:false;
}


PropertyMirror.prototype.attributes=function(){
	return %DebugPropertyAttributesFromDetails(this.details_);
}


PropertyMirror.prototype.propertyType=function(){
	return %DebugPropertyTypeFromDetails(this.details_);
}


PropertyMirror.prototype.insertionIndex=function(){
	return %DebugPropertyIndexFromDetails(this.details_);
}






PropertyMirror.prototype.hasGetter=function(){
	return this.getter_?true:false;
}






PropertyMirror.prototype.hasSetter=function(){
	return this.setter_?true:false;
}







PropertyMirror.prototype.getter=function(){
	if(this.hasGetter()){
		return MakeMirror(this.getter_);
	}else{
		return GetUndefinedMirror();
	}
}







PropertyMirror.prototype.setter=function(){
	if(this.hasSetter()){
		return MakeMirror(this.setter_);
	}else{
		return GetUndefinedMirror();
	}
}








PropertyMirror.prototype.isNative=function(){
	return(this.propertyType()==PropertyType.Interceptor)||
		((this.propertyType()==PropertyType.Callbacks)&&
		 !this.hasGetter()&&!this.hasSetter());
}


const kFrameDetailsFrameIdIndex=0;
const kFrameDetailsReceiverIndex=1;
const kFrameDetailsFunctionIndex=2;
const kFrameDetailsArgumentCountIndex=3;
const kFrameDetailsLocalCountIndex=4;
const kFrameDetailsSourcePositionIndex=5;
const kFrameDetailsConstructCallIndex=6;
const kFrameDetailsAtReturnIndex=7;
const kFrameDetailsDebuggerFrameIndex=8;
const kFrameDetailsFirstDynamicIndex=9;

const kFrameDetailsNameIndex=0;
const kFrameDetailsValueIndex=1;
const kFrameDetailsNameValueSize=2;





















function FrameDetails(a,b){
	this.break_id_=a;
	this.details_=%GetFrameDetails(a,b);
}


FrameDetails.prototype.frameId=function(){
	%CheckExecutionState(this.break_id_);
	return this.details_[kFrameDetailsFrameIdIndex];
}


FrameDetails.prototype.receiver=function(){
	%CheckExecutionState(this.break_id_);
	return this.details_[kFrameDetailsReceiverIndex];
}


FrameDetails.prototype.func=function(){
	%CheckExecutionState(this.break_id_);
	return this.details_[kFrameDetailsFunctionIndex];
}


FrameDetails.prototype.isConstructCall=function(){
	%CheckExecutionState(this.break_id_);
	return this.details_[kFrameDetailsConstructCallIndex];
}


FrameDetails.prototype.isAtReturn=function(){
	%CheckExecutionState(this.break_id_);
	return this.details_[kFrameDetailsAtReturnIndex];
}


FrameDetails.prototype.isDebuggerFrame=function(){
	%CheckExecutionState(this.break_id_);
	return this.details_[kFrameDetailsDebuggerFrameIndex];
}


FrameDetails.prototype.argumentCount=function(){
	%CheckExecutionState(this.break_id_);
	return this.details_[kFrameDetailsArgumentCountIndex];
}


FrameDetails.prototype.argumentName=function(a){
	%CheckExecutionState(this.break_id_);
	if(a>=0&&a<this.argumentCount()){
		return this.details_[kFrameDetailsFirstDynamicIndex+
			a*kFrameDetailsNameValueSize+
			kFrameDetailsNameIndex]
	}
}


FrameDetails.prototype.argumentValue=function(a){
	%CheckExecutionState(this.break_id_);
	if(a>=0&&a<this.argumentCount()){
		return this.details_[kFrameDetailsFirstDynamicIndex+
			a*kFrameDetailsNameValueSize+
			kFrameDetailsValueIndex]
	}
}


FrameDetails.prototype.localCount=function(){
	%CheckExecutionState(this.break_id_);
	return this.details_[kFrameDetailsLocalCountIndex];
}


FrameDetails.prototype.sourcePosition=function(){
	%CheckExecutionState(this.break_id_);
	return this.details_[kFrameDetailsSourcePositionIndex];
}


FrameDetails.prototype.localName=function(a){
	%CheckExecutionState(this.break_id_);
	if(a>=0&&a<this.localCount()){
		var b=kFrameDetailsFirstDynamicIndex+
			this.argumentCount()*kFrameDetailsNameValueSize
			return this.details_[b+
			a*kFrameDetailsNameValueSize+
			kFrameDetailsNameIndex]
	}
}


FrameDetails.prototype.localValue=function(a){
	%CheckExecutionState(this.break_id_);
	if(a>=0&&a<this.localCount()){
		var b=kFrameDetailsFirstDynamicIndex+
			this.argumentCount()*kFrameDetailsNameValueSize
			return this.details_[b+
			a*kFrameDetailsNameValueSize+
			kFrameDetailsValueIndex]
	}
}


FrameDetails.prototype.returnValue=function(){
	%CheckExecutionState(this.break_id_);
	var a=
		kFrameDetailsFirstDynamicIndex+
		(this.argumentCount()+this.localCount())*kFrameDetailsNameValueSize;
	if(this.details_[kFrameDetailsAtReturnIndex]){
		return this.details_[a];
	}
}


FrameDetails.prototype.scopeCount=function(){
	return %GetScopeCount(this.break_id_,this.frameId());
}










function FrameMirror(a,b){
	Mirror.call(this,FRAME_TYPE);
	this.break_id_=a;
	this.index_=b;
	this.details_=new FrameDetails(a,b);
}
inherits(FrameMirror,Mirror);


FrameMirror.prototype.index=function(){
	return this.index_;
};


FrameMirror.prototype.func=function(){

	var a=this.details_.func();




	if((%_IsFunction(a))){
		return MakeMirror(a);
	}else{
		return new UnresolvedFunctionMirror(a);
	}
};


FrameMirror.prototype.receiver=function(){
	return MakeMirror(this.details_.receiver());
};


FrameMirror.prototype.isConstructCall=function(){
	return this.details_.isConstructCall();
};


FrameMirror.prototype.isAtReturn=function(){
	return this.details_.isAtReturn();
};


FrameMirror.prototype.isDebuggerFrame=function(){
	return this.details_.isDebuggerFrame();
};


FrameMirror.prototype.argumentCount=function(){
	return this.details_.argumentCount();
};


FrameMirror.prototype.argumentName=function(a){
	return this.details_.argumentName(a);
};


FrameMirror.prototype.argumentValue=function(a){
	return MakeMirror(this.details_.argumentValue(a));
};


FrameMirror.prototype.localCount=function(){
	return this.details_.localCount();
};


FrameMirror.prototype.localName=function(a){
	return this.details_.localName(a);
};


FrameMirror.prototype.localValue=function(a){
	return MakeMirror(this.details_.localValue(a));
};


FrameMirror.prototype.returnValue=function(){
	return MakeMirror(this.details_.returnValue());
};


FrameMirror.prototype.sourcePosition=function(){
	return this.details_.sourcePosition();
};


FrameMirror.prototype.sourceLocation=function(){
	if(this.func().resolved()&&this.func().script()){
		return this.func().script().locationFromPosition(this.sourcePosition(),
				true);
	}
};


FrameMirror.prototype.sourceLine=function(){
	if(this.func().resolved()){
		var a=this.sourceLocation();
		if(a){
			return a.line;
		}
	}
};


FrameMirror.prototype.sourceColumn=function(){
	if(this.func().resolved()){
		var a=this.sourceLocation();
		if(a){
			return a.column;
		}
	}
};


FrameMirror.prototype.sourceLineText=function(){
	if(this.func().resolved()){
		var a=this.sourceLocation();
		if(a){
			return a.sourceText();
		}
	}
};


FrameMirror.prototype.scopeCount=function(){
	return this.details_.scopeCount();
};


FrameMirror.prototype.scope=function(a){
	return new ScopeMirror(this,a);
};


FrameMirror.prototype.evaluate=function(a,b){
	var d=%DebugEvaluate(this.break_id_,this.details_.frameId(),
			a,Boolean(b));
	return MakeMirror(d);
};


FrameMirror.prototype.invocationText=function(){

	var a='';
	var b=this.func();
	var d=this.receiver();
	if(this.isConstructCall()){

		a+='new ';
		a+=b.name()?b.name():'[anonymous]';
	}else if(this.isDebuggerFrame()){
		a+='[debugger]';
	}else{

		var g=!d.className||d.className()!='global';
		if(g){
			a+=d.toText();
		}


		var h=GetUndefinedMirror();
		if(!d.isUndefined()){
			for(var i=d;!i.isNull()&&h.isUndefined();i=i.protoObject()){
				h=i.lookupProperty(b);
			}
		}
		if(!h.isUndefined()){


			if(!h.isIndexed()){
				if(g){
					a+='.';
				}
				a+=h.name();
			}else{
				a+='[';
				a+=h.name();
				a+=']';
			}


			if(b.name()&&b.name()!=h.name()){
				a+='(aka '+b.name()+')';
						}
						}else{


							if(g){
								a+='.';
							}
							a+=b.name()?b.name():'[anonymous]';
						}
						}


						if(!this.isDebuggerFrame()){
							a+='(';
							for(var j=0;j<this.argumentCount();j++){
								if(j!=0)a+=', ';
								if(this.argumentName(j)){
									a+=this.argumentName(j);
									a+='=';
								}
								a+=this.argumentValue(j).toText();
							}
							a+=')';
						}

						if(this.isAtReturn()){
							a+=' returning ';
							a+=this.returnValue().toText();
						}

						return a;
}


FrameMirror.prototype.sourceAndPositionText=function(){

	var a='';
	var b=this.func();
	if(b.resolved()){
		if(b.script()){
			if(b.script().name()){
				a+=b.script().name();
			}else{
				a+='[unnamed]';
			}
			if(!this.isDebuggerFrame()){
				var d=this.sourceLocation();
				a+=' line ';
				a+=!(typeof(d)==='undefined')?(d.line+1):'?';
				a+=' column ';
				a+=!(typeof(d)==='undefined')?(d.column+1):'?';
				if(!(typeof(this.sourcePosition())==='undefined')){
					a+=' (position '+(this.sourcePosition()+1)+')';
							}
							}
							}else{
								a+='[no source]';
							}
							}else{
								a+='[unresolved]';
							}

							return a;
							}


							FrameMirror.prototype.localsText=function(){

								var a='';
								var b=this.localCount()
						if(b>0){
							for(var d=0;d<b;++d){
								a+='      var ';
								a+=this.localName(d);
								a+=' = ';
								a+=this.localValue(d).toText();
								if(d<b-1)a+='\n';
							}
						}

					return a;
							}


FrameMirror.prototype.toText=function(a){
	var b='';
	b+='#'+(this.index()<=9?'0':'')+this.index();
	b+=' ';
	b+=this.invocationText();
	b+=' ';
	b+=this.sourceAndPositionText();
	if(a){
		b+='\n';
		b+=this.localsText();
	}
	return b;
}


const kScopeDetailsTypeIndex=0;
const kScopeDetailsObjectIndex=1;

function ScopeDetails(a,b){
	this.break_id_=a.break_id_;
	this.details_=%GetScopeDetails(a.break_id_,
			a.details_.frameId(),
			b);
}


ScopeDetails.prototype.type=function(){
	%CheckExecutionState(this.break_id_);
	return this.details_[kScopeDetailsTypeIndex];
}


ScopeDetails.prototype.object=function(){
	%CheckExecutionState(this.break_id_);
	return this.details_[kScopeDetailsObjectIndex];
}









function ScopeMirror(a,b){
	Mirror.call(this,SCOPE_TYPE);
	this.frame_index_=a.index_;
	this.scope_index_=b;
	this.details_=new ScopeDetails(a,b);
}
inherits(ScopeMirror,Mirror);


ScopeMirror.prototype.frameIndex=function(){
	return this.frame_index_;
};


ScopeMirror.prototype.scopeIndex=function(){
	return this.scope_index_;
};


ScopeMirror.prototype.scopeType=function(){
	return this.details_.type();
};


ScopeMirror.prototype.scopeObject=function(){



	var a=this.scopeType()==ScopeType.Local||
		this.scopeType()==ScopeType.Closure;
	return MakeMirror(this.details_.object(),a);
};








function ScriptMirror(a){
	Mirror.call(this,SCRIPT_TYPE);
	this.script_=a;
	this.context_=new ContextMirror(a.context_data);
	this.allocateHandle_();
}
inherits(ScriptMirror,Mirror);


ScriptMirror.prototype.value=function(){
	return this.script_;
};


ScriptMirror.prototype.name=function(){
	return this.script_.name||this.script_.nameOrSourceURL();
};


ScriptMirror.prototype.id=function(){
	return this.script_.id;
};


ScriptMirror.prototype.source=function(){
	return this.script_.source;
};


ScriptMirror.prototype.lineOffset=function(){
	return this.script_.line_offset;
};


ScriptMirror.prototype.columnOffset=function(){
	return this.script_.column_offset;
};


ScriptMirror.prototype.data=function(){
	return this.script_.data;
};


ScriptMirror.prototype.scriptType=function(){
	return this.script_.type;
};


ScriptMirror.prototype.compilationType=function(){
	return this.script_.compilation_type;
};


ScriptMirror.prototype.lineCount=function(){
	return this.script_.lineCount();
};


ScriptMirror.prototype.locationFromPosition=function(
		position,include_resource_offset){
			return this.script_.locationFromPosition(position,include_resource_offset);
		}


ScriptMirror.prototype.sourceSlice=function(a,b){
	return this.script_.sourceSlice(a,b);
}


ScriptMirror.prototype.context=function(){
	return this.context_;
};


ScriptMirror.prototype.evalFromScript=function(){
	return MakeMirror(this.script_.eval_from_script);
};


ScriptMirror.prototype.evalFromFunctionName=function(){
	return MakeMirror(this.script_.eval_from_function_name);
};


ScriptMirror.prototype.evalFromLocation=function(){
	var a=this.evalFromScript();
	if(!a.isUndefined()){
		var b=this.script_.eval_from_script_position;
		return a.locationFromPosition(b,true);
	}
};


ScriptMirror.prototype.toText=function(){
	var a='';
	a+=this.name();
	a+=' (lines: ';
			if(this.lineOffset()>0){
				a+=this.lineOffset();
				a+='-';
				a+=this.lineOffset()+this.lineCount()-1;
			}else{
				a+=this.lineCount();
			}
			a+=')';
			return a;
			}








			function ContextMirror(a){
				Mirror.call(this,CONTEXT_TYPE);
				this.data_=a;
				this.allocateHandle_();
			}
inherits(ContextMirror,Mirror);


ContextMirror.prototype.data=function(){
	return this.data_;
};











function MakeMirrorSerializer(a,b){
	return new JSONProtocolSerializer(a,b);
}








function JSONProtocolSerializer(a,b){
	this.details_=a;
	this.options_=b;
	this.mirrors_=[];
}









JSONProtocolSerializer.prototype.serializeReference=function(a){
	return this.serialize_(a,true,true);
}









JSONProtocolSerializer.prototype.serializeValue=function(a){
	var b=this.serialize_(a,false,true);
	return b;
}









JSONProtocolSerializer.prototype.serializeReferencedObjects=function(){

	var a=[];


	var b=this.mirrors_.length;

	for(var d=0;d<b;d++){
		a.push(this.serialize_(this.mirrors_[d],false,false));
	}

	return a;
}


JSONProtocolSerializer.prototype.includeSource_=function(){
	return this.options_&&this.options_.includeSource;
}


JSONProtocolSerializer.prototype.inlineRefs_=function(){
	return this.options_&&this.options_.inlineRefs;
}


JSONProtocolSerializer.prototype.maxStringLength_=function(){
	if((typeof(this.options_)==='undefined')||
			(typeof(this.options_.maxStringLength)==='undefined')){
				return kMaxProtocolStringLength;
			}
	return this.options_.maxStringLength;
}


JSONProtocolSerializer.prototype.add_=function(a){

	for(var b=0;b<this.mirrors_.length;b++){
		if(this.mirrors_[b]===a){
			return;
		}
	}


	this.mirrors_.push(a);
}








JSONProtocolSerializer.prototype.serializeReferenceWithDisplayData_=
function(a){
	var b={};
	b.ref=a.handle();
	b.type=a.type();
	switch(a.type()){
		case UNDEFINED_TYPE:
		case NULL_TYPE:
		case BOOLEAN_TYPE:
		case NUMBER_TYPE:
			b.value=a.value();
			break;
		case STRING_TYPE:
			b.value=a.getTruncatedValue(this.maxStringLength_());
			break;
		case FUNCTION_TYPE:
			b.name=a.name();
			b.inferredName=a.inferredName();
			if(a.script()){
				b.scriptId=a.script().id();
			}
			break;
		case ERROR_TYPE:
		case REGEXP_TYPE:
			b.value=a.toText();
			break;
		case OBJECT_TYPE:
			b.className=a.className();
			break;
	}
	return b;
};


JSONProtocolSerializer.prototype.serialize_=function(mirror,reference,
		details){


			if(reference&&
					(mirror.isValue()||mirror.isScript()||mirror.isContext())){
						if(this.inlineRefs_()&&mirror.isValue()){
							return this.serializeReferenceWithDisplayData_(mirror);
						}else{
							this.add_(mirror);
							return{'ref':mirror.handle()};
						}
					}


			var a={};


			if(mirror.isValue()||mirror.isScript()||mirror.isContext()){
				a.handle=mirror.handle();
			}


			a.type=mirror.type();

			switch(mirror.type()){
				case UNDEFINED_TYPE:
				case NULL_TYPE:

					break;

				case BOOLEAN_TYPE:

					a.value=mirror.value();
					break;

				case NUMBER_TYPE:

					a.value=NumberToJSON_(mirror.value());
					break;

				case STRING_TYPE:

					if(this.maxStringLength_()!=-1&&
							mirror.length()>this.maxStringLength_()){
								var b=mirror.getTruncatedValue(this.maxStringLength_());
								a.value=b;
								a.fromIndex=0;
								a.toIndex=this.maxStringLength_();
							}else{
								a.value=mirror.value();
							}
					a.length=mirror.length();
					break;

				case OBJECT_TYPE:
				case FUNCTION_TYPE:
				case ERROR_TYPE:
				case REGEXP_TYPE:

					this.serializeObject_(mirror,a,details);
					break;

				case PROPERTY_TYPE:
					throw new Error('PropertyMirror cannot be serialized independeltly')
						break;

				case FRAME_TYPE:

					this.serializeFrame_(mirror,a);
					break;

				case SCOPE_TYPE:

					this.serializeScope_(mirror,a);
					break;

				case SCRIPT_TYPE:

					if(mirror.name()){
						a.name=mirror.name();
					}
					a.id=mirror.id();
					a.lineOffset=mirror.lineOffset();
					a.columnOffset=mirror.columnOffset();
					a.lineCount=mirror.lineCount();
					if(mirror.data()){
						a.data=mirror.data();
					}
					if(this.includeSource_()){
						a.source=mirror.source();
					}else{
						var d=mirror.source().substring(0,80);
						a.sourceStart=d;
					}
					a.sourceLength=mirror.source().length;
					a.scriptType=mirror.scriptType();
					a.compilationType=mirror.compilationType();


					if(mirror.compilationType()==1&&
							mirror.evalFromScript()){
								a.evalFromScript=
									this.serializeReference(mirror.evalFromScript());
								var g=mirror.evalFromLocation()
									if(g){
										a.evalFromLocation={line:g.line,
											column:g.column};
									}
								if(mirror.evalFromFunctionName()){
									a.evalFromFunctionName=mirror.evalFromFunctionName();
								}
							}
					if(mirror.context()){
						a.context=this.serializeReference(mirror.context());
					}
					break;

				case CONTEXT_TYPE:
					a.data=mirror.data();
					break;
			}


			a.text=mirror.toText();


			return a;
		}













JSONProtocolSerializer.prototype.serializeObject_=function(mirror,content,
		details){

			content.className=mirror.className();
			content.constructorFunction=
				this.serializeReference(mirror.constructorFunction());
			content.protoObject=this.serializeReference(mirror.protoObject());
			content.prototypeObject=this.serializeReference(mirror.prototypeObject());


			if(mirror.hasNamedInterceptor()){
				content.namedInterceptor=true;
			}
			if(mirror.hasIndexedInterceptor()){
				content.indexedInterceptor=true;
			}


			if(mirror.isFunction()){

				content.name=mirror.name();
				if(!(typeof(mirror.inferredName())==='undefined')){
					content.inferredName=mirror.inferredName();
				}
				content.resolved=mirror.resolved();
				if(mirror.resolved()){
					content.source=mirror.source();
				}
				if(mirror.script()){
					content.script=this.serializeReference(mirror.script());
					content.scriptId=mirror.script().id();

					serializeLocationFields(mirror.sourceLocation(),content);
				}
			}


			if(mirror.isDate()){

				content.value=mirror.value();
			}


			var a=mirror.propertyNames(PropertyKind.Named);
			var b=mirror.propertyNames(PropertyKind.Indexed);
			var d=new Array(a.length+b.length);
			for(var g=0;g<a.length;g++){
				var h=mirror.property(a[g]);
				d[g]=this.serializeProperty_(h);
				if(details){
					this.add_(h.value());
				}
			}
			for(var g=0;g<b.length;g++){
				var h=mirror.property(b[g]);
				d[a.length+g]=this.serializeProperty_(h);
				if(details){
					this.add_(h.value());
				}
			}
			content.properties=d;
		}











function serializeLocationFields(a,b){
	if(!a){
		return;
	}
	b.position=a.position;
	var d=a.line;
	if(!(typeof(d)==='undefined')){
		b.line=d;
	}
	var g=a.column;
	if(!(typeof(g)==='undefined')){
		b.column=g;
	}
}





















JSONProtocolSerializer.prototype.serializeProperty_=function(a){
	var b={};

	b.name=a.name();
	var d=a.value();
	if(this.inlineRefs_()&&d.isValue()){
		b.value=this.serializeReferenceWithDisplayData_(d);
	}else{
		if(a.attributes()!=PropertyAttribute.None){
			b.attributes=a.attributes();
		}
		if(a.propertyType()!=PropertyType.Normal){
			b.propertyType=a.propertyType();
		}
		b.ref=d.handle();
	}
	return b;
}


JSONProtocolSerializer.prototype.serializeFrame_=function(a,b){
	b.index=a.index();
	b.receiver=this.serializeReference(a.receiver());
	var d=a.func();
	b.func=this.serializeReference(d);
	if(d.script()){
		b.script=this.serializeReference(d.script());
	}
	b.constructCall=a.isConstructCall();
	b.atReturn=a.isAtReturn();
	if(a.isAtReturn()){
		b.returnValue=this.serializeReference(a.returnValue());
	}
	b.debuggerFrame=a.isDebuggerFrame();
	var g=new Array(a.argumentCount());
	for(var h=0;h<a.argumentCount();h++){
		var i={};
		var j=a.argumentName(h)
			if(j){
				i.name=j;
			}
		i.value=this.serializeReference(a.argumentValue(h));
		g[h]=i;
	}
	b.arguments=g;
	var g=new Array(a.localCount());
	for(var h=0;h<a.localCount();h++){
		var k={};
		k.name=a.localName(h);
		k.value=this.serializeReference(a.localValue(h));
		g[h]=k;
	}
	b.locals=g;
	serializeLocationFields(a.sourceLocation(),b);
	var l=a.sourceLineText();
	if(!(typeof(l)==='undefined')){
		b.sourceLineText=l;
	}

	b.scopes=[];
	for(var h=0;h<a.scopeCount();h++){
		var m=a.scope(h);
		b.scopes.push({
			type:m.scopeType(),
			index:h
		});
	}
}


JSONProtocolSerializer.prototype.serializeScope_=function(a,b){
	b.index=a.scopeIndex();
	b.frameIndex=a.frameIndex();
	b.type=a.scopeType();
	b.object=this.inlineRefs_()?
		this.serializeValue(a.scopeObject()):
		this.serializeReference(a.scopeObject());
}











function NumberToJSON_(a){
	if(isNaN(a)){
		return'NaN';
	}
	if(!isFinite(a)){
		if(a>0){
			return'Infinity';
		}else{
			return'-Infinity';
		}
	}
	return a;
}









































const $Date=global.Date;


function ThrowDateTypeError(){
	throw new $TypeError('this is not a Date object.');
}


function Modulo(a,b){
	var d=a % b;

	if(d==0)return 0;
	return d>=0?d:d+b;
}


function TimeWithinDay(a){
	return Modulo(a,86400000);
}



function DaysInYear(a){
	if(a % 4!=0)return 365;
	if((a % 100==0)&&(a % 400!=0))return 365;
	return 366;
}


function DayFromYear(a){
	return 365*(a-1970)
		+$floor((a-1969)/4)
		-$floor((a-1901)/100)
		+$floor((a-1601)/400);
}


function TimeFromYear(a){
	return 86400000*DayFromYear(a);
}


function InLeapYear(a){
	return DaysInYear((YearFromTime(a)))==366?1:0;
}


function DayWithinYear(a){
	return($floor(a/86400000))-DayFromYear((YearFromTime(a)));
}



function EquivalentYear(a){



	var b=TimeFromYear(a);
	var d=(InLeapYear(b)==0?1967:1956)+
		(WeekDay(b)*12)% 28;


	return 2008+(d+3*28-2008)% 28;
}


function EquivalentTime(a){







	if(a>=0&&a<=2.1e12)return a;

	var b=MakeDay(EquivalentYear((YearFromTime(a))),
			(MonthFromTime(a)),
			(DateFromTime(a)));
	return MakeDate(b,TimeWithinDay(a));
}





var local_time_offset;






var DST_offset_cache={

	offset:0,

	start:0,end:-1,

	increment:0,
	initial_increment:19*86400000
};










function DaylightSavingsOffset(a){

	var b=DST_offset_cache;


	var d=b.start;
	var g=b.end;

	if(d<=a){

		if(a<=g)return b.offset;


		if((typeof(local_time_offset)==='undefined')){
			local_time_offset=%DateLocalTimeOffset();
		}


		var h=g+b.increment;

		if(a<=h){
			var i=%DateDaylightSavingsOffset(EquivalentTime(h));
			if(b.offset==i){



				b.end=h;
				b.increment=b.initial_increment;
				return i;
			}else{
				var j=%DateDaylightSavingsOffset(EquivalentTime(a));
				if(j==i){




					b.start=a;
					b.end=h;
					b.increment=b.initial_increment;
				}else{



					b.increment/=3;
					b.end=a;
				}

				b.offset=j;
				return j;
			}
		}
	}


	if((typeof(local_time_offset)==='undefined')){
		local_time_offset=%DateLocalTimeOffset();
	}



	var j=%DateDaylightSavingsOffset(EquivalentTime(a));
	b.offset=j;
	b.start=b.end=a;
	b.increment=b.initial_increment;
	return j;
}


var timezone_cache_time=$NaN;
var timezone_cache_timezone;

function LocalTimezone(a){
	if((!%_IsSmi(%IS_VAR(a))&&!(a==a)))return"";
	if(a==timezone_cache_time){
		return timezone_cache_timezone;
	}
	var b=%DateLocalTimezone(EquivalentTime(a));
	timezone_cache_time=a;
	timezone_cache_timezone=b;
	return b;
}


function WeekDay(a){
	return Modulo(($floor(a/86400000))+4,7);
}


function LocalTime(a){
	if((!%_IsSmi(%IS_VAR(a))&&!(a==a)))return a;

	return a+DaylightSavingsOffset(a)+local_time_offset;
}


var ltcache={
	key:null,
	val:null
};

function LocalTimeNoCheck(a){
	var b=ltcache;
	if(%_ObjectEquals(a,b.key))return b.val;
	if(a<-8640000000000000||a>8640000000000000){
		return $NaN;
	}




	var d=DST_offset_cache;
	if(d.start<=a&&a<=d.end){
		var g=d.offset;
	}else{
		var g=DaylightSavingsOffset(a);
	}
	b.key=a;
	return(b.val=a+local_time_offset+g);
}


function UTC(a){
	if((!%_IsSmi(%IS_VAR(a))&&!(a==a)))return a;


	if((typeof(local_time_offset)==='undefined')){
		local_time_offset=%DateLocalTimeOffset();
	}
	var b=a-local_time_offset;
	return b-DaylightSavingsOffset(b);
}



function MakeTime(a,b,d,g){
	if(!$isFinite(a))return $NaN;
	if(!$isFinite(b))return $NaN;
	if(!$isFinite(d))return $NaN;
	if(!$isFinite(g))return $NaN;
	return(%_IsSmi(%IS_VAR(a))?a:%NumberToInteger(ToNumber(a)))*3600000
		+(%_IsSmi(%IS_VAR(b))?b:%NumberToInteger(ToNumber(b)))*60000
		+(%_IsSmi(%IS_VAR(d))?d:%NumberToInteger(ToNumber(d)))*1000
		+(%_IsSmi(%IS_VAR(g))?g:%NumberToInteger(ToNumber(g)));
}



function TimeInYear(a){
	return DaysInYear(a)*86400000;
}


var ymd_from_time_cache=[$NaN,$NaN,$NaN];
var ymd_from_time_cached_time=$NaN;

function YearFromTime(a){
	if(a!==ymd_from_time_cached_time){
		if(!$isFinite(a)){
			return $NaN;
		}

		%DateYMDFromTime(a,ymd_from_time_cache);
		ymd_from_time_cached_time=a
	}

	return ymd_from_time_cache[0];
}

function MonthFromTime(a){
	if(a!==ymd_from_time_cached_time){
		if(!$isFinite(a)){
			return $NaN;
		}
		%DateYMDFromTime(a,ymd_from_time_cache);
		ymd_from_time_cached_time=a
	}

	return ymd_from_time_cache[1];
}

function DateFromTime(a){
	if(a!==ymd_from_time_cached_time){
		if(!$isFinite(a)){
			return $NaN;
		}

		%DateYMDFromTime(a,ymd_from_time_cache);
		ymd_from_time_cached_time=a
	}

	return ymd_from_time_cache[2];
}








function MakeDay(a,b,d){
	if(!$isFinite(a)||!$isFinite(b)||!$isFinite(d))return $NaN;


	a=(%_IsSmi(%IS_VAR(a))?a:%NumberToIntegerMapMinusZero(ToNumber(a)));
	b=(%_IsSmi(%IS_VAR(b))?b:%NumberToIntegerMapMinusZero(ToNumber(b)));
	d=(%_IsSmi(%IS_VAR(d))?d:%NumberToIntegerMapMinusZero(ToNumber(d)));

	if(a<-1000000||a>1000000||
			b<-10000000||b>10000000||
			d<-100000000||d>100000000){
				return $NaN;
			}


	return %DateMakeDay(a,b,d);
}



function MakeDate(a,b){
	if(!$isFinite(a))return $NaN;
	if(!$isFinite(b))return $NaN;
	return a*86400000+b;
}



function TimeClip(a){
	if(!$isFinite(a))return $NaN;
	if($abs(a)>8.64E15)return $NaN;
	return(%_IsSmi(%IS_VAR(a))?a:%NumberToInteger(ToNumber(a)));
}




var Date_cache={

	time:$NaN,


	year:$NaN,

	string:null
};


%SetCode($Date,function(a,b,d,g,h,i,j){
	if(!%_IsConstructCall()){

		return(new $Date()).toString();
	}


	var k=%_ArgumentsLength();
	var l;
	if(k==0){
		l=%DateCurrentTime();

	}else if(k==1){
		if((typeof(a)==='number')){
			l=TimeClip(a);

		}else if((typeof(a)==='string')){


			var m=Date_cache;
			if(m.string===a){
				l=m.time;
			}else{
				l=DateParse(a);
				if(!(!%_IsSmi(%IS_VAR(l))&&!(l==l))){
					m.time=l;
					m.year=(YearFromTime(LocalTimeNoCheck(l)));
					m.string=a;
				}
			}

		}else{







			var o=ToPrimitive(a,1);
			l=(typeof(o)==='string')?DateParse(o):TimeClip(ToNumber(o));
		}

	}else{
		a=ToNumber(a);
		b=ToNumber(b);
		d=k>2?ToNumber(d):1;
		g=k>3?ToNumber(g):0;
		h=k>4?ToNumber(h):0;
		i=k>5?ToNumber(i):0;
		j=k>6?ToNumber(j):0;
		a=(!(!%_IsSmi(%IS_VAR(a))&&!(a==a))&&0<=(%_IsSmi(%IS_VAR(a))?a:%NumberToInteger(ToNumber(a)))&&(%_IsSmi(%IS_VAR(a))?a:%NumberToInteger(ToNumber(a)))<=99)
			?1900+(%_IsSmi(%IS_VAR(a))?a:%NumberToInteger(ToNumber(a))):a;
		var q=MakeDay(a,b,d);
		var o=MakeTime(g,h,i,j);
		l=TimeClip(UTC(MakeDate(q,o)));
	}
	%_SetValueOf(this,l);
});


%FunctionSetPrototype($Date,new $Date($NaN));


var WeekDays=['Sun','Mon','Tue','Wed','Thu','Fri','Sat'];
var Months=['Jan','Feb','Mar','Apr','May','Jun','Jul','Aug','Sep','Oct','Nov','Dec'];


function TwoDigitString(a){
	return a<10?"0"+a:""+a;
}


function DateString(a){
	return WeekDays[WeekDay(a)]+' '
		+Months[MonthFromTime(a)]+' '
		+TwoDigitString(DateFromTime(a))+' '
		+YearFromTime(a);
}


var LongWeekDays=['Sunday','Monday','Tuesday','Wednesday','Thursday','Friday','Saturday'];
var LongMonths=['January','February','March','April','May','June','July','August','September','October','November','December'];


function LongDateString(a){
	return LongWeekDays[WeekDay(a)]+', '
		+LongMonths[MonthFromTime(a)]+' '
		+TwoDigitString(DateFromTime(a))+', '
		+YearFromTime(a);
}


function TimeString(a){
	return TwoDigitString((Modulo($floor(a/3600000),24)))+':'
		+TwoDigitString((Modulo($floor(a/60000),60)))+':'
		+TwoDigitString((Modulo($floor(a/1000),60)));
}


function LocalTimezoneString(a){
	var b=timezone_cache_timezone;
	var d=LocalTimezone(a);
	if(b&&d!=b){



		local_time_offset=%DateLocalTimeOffset();


		var g=DST_offset_cache;
		g.start=0;
		g.end=-1;
	}

	var h=
		(DaylightSavingsOffset(a)+local_time_offset)/60000;
	var i=(h>=0)?1:-1;
	var j=$floor((i*h)/60);
	var k=$floor((i*h)%60);
	var l=' GMT'+((i==1)?'+':'-')+
		TwoDigitString(j)+TwoDigitString(k);
	return l+' ('+d+')';
			}


			function DatePrintString(a){
				return DateString(a)+' '+TimeString(a);
			}




			var parse_buffer=$Array(8);


			function DateParse(a){
				var b=%DateParseString(ToString(a),parse_buffer);
				if((b===null))return $NaN;

				var d=MakeDay(b[0],b[1],b[2]);
				var g=MakeTime(b[3],b[4],b[5],b[6]);
				var h=MakeDate(d,g);

				if((b[7]===null)){
					return TimeClip(UTC(h));
				}else{
					return TimeClip(h-b[7]*1000);
				}
			}



function DateUTC(a,b,d,g,h,i,j){
	a=ToNumber(a);
	b=ToNumber(b);
	var k=%_ArgumentsLength();
	d=k>2?ToNumber(d):1;
	g=k>3?ToNumber(g):0;
	h=k>4?ToNumber(h):0;
	i=k>5?ToNumber(i):0;
	j=k>6?ToNumber(j):0;
	a=(!(!%_IsSmi(%IS_VAR(a))&&!(a==a))&&0<=(%_IsSmi(%IS_VAR(a))?a:%NumberToInteger(ToNumber(a)))&&(%_IsSmi(%IS_VAR(a))?a:%NumberToInteger(ToNumber(a)))<=99)
		?1900+(%_IsSmi(%IS_VAR(a))?a:%NumberToInteger(ToNumber(a))):a;
	var l=MakeDay(a,b,d);
	var m=MakeTime(g,h,i,j);
	return %_SetValueOf(this,TimeClip(MakeDate(l,m)));
}




function DateNow(){
	return %DateCurrentTime();
}



function DateToString(){
	var a=(%_ClassOf(this)==='Date'?%_ValueOf(this):ThrowDateTypeError());
	if((!%_IsSmi(%IS_VAR(a))&&!(a==a)))return'Invalid Date';
	var b=LocalTimezoneString(a);
	return DatePrintString(LocalTimeNoCheck(a))+b;
}



function DateToDateString(){
	var a=(%_ClassOf(this)==='Date'?%_ValueOf(this):ThrowDateTypeError());
	if((!%_IsSmi(%IS_VAR(a))&&!(a==a)))return'Invalid Date';
	return DateString(LocalTimeNoCheck(a));
}



function DateToTimeString(){
	var a=(%_ClassOf(this)==='Date'?%_ValueOf(this):ThrowDateTypeError());
	if((!%_IsSmi(%IS_VAR(a))&&!(a==a)))return'Invalid Date';
	var b=LocalTimezoneString(a);
	return TimeString(LocalTimeNoCheck(a))+b;
}



function DateToLocaleString(){
	return DateToString.call(this);
}



function DateToLocaleDateString(){
	var a=(%_ClassOf(this)==='Date'?%_ValueOf(this):ThrowDateTypeError());
	if((!%_IsSmi(%IS_VAR(a))&&!(a==a)))return'Invalid Date';
	return LongDateString(LocalTimeNoCheck(a));
}



function DateToLocaleTimeString(){
	var a=(%_ClassOf(this)==='Date'?%_ValueOf(this):ThrowDateTypeError());
	if((!%_IsSmi(%IS_VAR(a))&&!(a==a)))return'Invalid Date';
	var b=LocalTimeNoCheck(a);
	return TimeString(b);
}



function DateValueOf(){
	return(%_ClassOf(this)==='Date'?%_ValueOf(this):ThrowDateTypeError());
}



function DateGetTime(){
	return(%_ClassOf(this)==='Date'?%_ValueOf(this):ThrowDateTypeError());
}



function DateGetFullYear(){
	var a=(%_ClassOf(this)==='Date'?%_ValueOf(this):ThrowDateTypeError());
	if((!%_IsSmi(%IS_VAR(a))&&!(a==a)))return a;
	var b=Date_cache;
	if(b.time===a)return b.year;
	return(YearFromTime(LocalTimeNoCheck(a)));
}



function DateGetUTCFullYear(){
	var a=(%_ClassOf(this)==='Date'?%_ValueOf(this):ThrowDateTypeError());
	if((!%_IsSmi(%IS_VAR(a))&&!(a==a)))return a;
	return(YearFromTime(a));
}



function DateGetMonth(){
	var a=(%_ClassOf(this)==='Date'?%_ValueOf(this):ThrowDateTypeError());
	if((!%_IsSmi(%IS_VAR(a))&&!(a==a)))return a;
	return(MonthFromTime(LocalTimeNoCheck(a)));
}



function DateGetUTCMonth(){
	var a=(%_ClassOf(this)==='Date'?%_ValueOf(this):ThrowDateTypeError());
	if((!%_IsSmi(%IS_VAR(a))&&!(a==a)))return a;
	return(MonthFromTime(a));
}



function DateGetDate(){
	var a=(%_ClassOf(this)==='Date'?%_ValueOf(this):ThrowDateTypeError());
	if((!%_IsSmi(%IS_VAR(a))&&!(a==a)))return a;
	return(DateFromTime(LocalTimeNoCheck(a)));
}



function DateGetUTCDate(){
	var a=(%_ClassOf(this)==='Date'?%_ValueOf(this):ThrowDateTypeError());
	return((!%_IsSmi(%IS_VAR(a))&&!(a==a))?a:(DateFromTime(a)));
}



function DateGetDay(){
	var a=%_ValueOf(this);
	if((!%_IsSmi(%IS_VAR(a))&&!(a==a)))return a;
	return WeekDay(LocalTimeNoCheck(a));
}



function DateGetUTCDay(){
	var a=%_ValueOf(this);
	if((!%_IsSmi(%IS_VAR(a))&&!(a==a)))return a;
	return WeekDay(a);
}



function DateGetHours(){
	var a=(%_ClassOf(this)==='Date'?%_ValueOf(this):ThrowDateTypeError());
	if((!%_IsSmi(%IS_VAR(a))&&!(a==a)))return a;
	return(Modulo($floor(LocalTimeNoCheck(a)/3600000),24));
}



function DateGetUTCHours(){
	var a=(%_ClassOf(this)==='Date'?%_ValueOf(this):ThrowDateTypeError());
	if((!%_IsSmi(%IS_VAR(a))&&!(a==a)))return a;
	return(Modulo($floor(a/3600000),24));
}



function DateGetMinutes(){
	var a=(%_ClassOf(this)==='Date'?%_ValueOf(this):ThrowDateTypeError());
	if((!%_IsSmi(%IS_VAR(a))&&!(a==a)))return a;
	return(Modulo($floor(LocalTimeNoCheck(a)/60000),60));
}



function DateGetUTCMinutes(){
	var a=(%_ClassOf(this)==='Date'?%_ValueOf(this):ThrowDateTypeError());
	return((!%_IsSmi(%IS_VAR(a))&&!(a==a))?a:(Modulo($floor(a/60000),60)));
}



function DateGetSeconds(){
	var a=(%_ClassOf(this)==='Date'?%_ValueOf(this):ThrowDateTypeError());
	if((!%_IsSmi(%IS_VAR(a))&&!(a==a)))return a;
	return(Modulo($floor(LocalTimeNoCheck(a)/1000),60));
}



function DateGetUTCSeconds(){
	var a=(%_ClassOf(this)==='Date'?%_ValueOf(this):ThrowDateTypeError());
	return((!%_IsSmi(%IS_VAR(a))&&!(a==a))?a:(Modulo($floor(a/1000),60)));
}



function DateGetMilliseconds(){
	var a=(%_ClassOf(this)==='Date'?%_ValueOf(this):ThrowDateTypeError());
	if((!%_IsSmi(%IS_VAR(a))&&!(a==a)))return a;
	return(Modulo(LocalTimeNoCheck(a),1000));
}



function DateGetUTCMilliseconds(){
	var a=(%_ClassOf(this)==='Date'?%_ValueOf(this):ThrowDateTypeError());
	return((!%_IsSmi(%IS_VAR(a))&&!(a==a))?a:(Modulo(a,1000)));
}



function DateGetTimezoneOffset(){
	var a=(%_ClassOf(this)==='Date'?%_ValueOf(this):ThrowDateTypeError());
	if((!%_IsSmi(%IS_VAR(a))&&!(a==a)))return a;
	return(a-LocalTimeNoCheck(a))/60000;
}



function DateSetTime(a){
	if(!(%_ClassOf(this)==='Date'))ThrowDateTypeError();
	return %_SetValueOf(this,TimeClip(ToNumber(a)));
}



function DateSetMilliseconds(a){
	var b=LocalTime((%_ClassOf(this)==='Date'?%_ValueOf(this):ThrowDateTypeError()));
	a=ToNumber(a);
	var d=MakeTime((Modulo($floor(b/3600000),24)),(Modulo($floor(b/60000),60)),(Modulo($floor(b/1000),60)),a);
	return %_SetValueOf(this,TimeClip(UTC(MakeDate(($floor(b/86400000)),d))));
}



function DateSetUTCMilliseconds(a){
	var b=(%_ClassOf(this)==='Date'?%_ValueOf(this):ThrowDateTypeError());
	a=ToNumber(a);
	var d=MakeTime((Modulo($floor(b/3600000),24)),(Modulo($floor(b/60000),60)),(Modulo($floor(b/1000),60)),a);
	return %_SetValueOf(this,TimeClip(MakeDate(($floor(b/86400000)),d)));
}



function DateSetSeconds(a,b){
	var d=LocalTime((%_ClassOf(this)==='Date'?%_ValueOf(this):ThrowDateTypeError()));
	a=ToNumber(a);
	b=%_ArgumentsLength()<2?((!%_IsSmi(%IS_VAR(d))&&!(d==d))?d:(Modulo(d,1000))):ToNumber(b);
	var g=MakeTime((Modulo($floor(d/3600000),24)),(Modulo($floor(d/60000),60)),a,b);
	return %_SetValueOf(this,TimeClip(UTC(MakeDate(($floor(d/86400000)),g))));
}



function DateSetUTCSeconds(a,b){
	var d=(%_ClassOf(this)==='Date'?%_ValueOf(this):ThrowDateTypeError());
	a=ToNumber(a);
	b=%_ArgumentsLength()<2?((!%_IsSmi(%IS_VAR(d))&&!(d==d))?d:(Modulo(d,1000))):ToNumber(b);
	var g=MakeTime((Modulo($floor(d/3600000),24)),(Modulo($floor(d/60000),60)),a,b);
	return %_SetValueOf(this,TimeClip(MakeDate(($floor(d/86400000)),g)));
}



function DateSetMinutes(a,b,d){
	var g=LocalTime((%_ClassOf(this)==='Date'?%_ValueOf(this):ThrowDateTypeError()));
	a=ToNumber(a);
	var h=%_ArgumentsLength();
	b=h<2?((!%_IsSmi(%IS_VAR(g))&&!(g==g))?g:(Modulo($floor(g/1000),60))):ToNumber(b);
	d=h<3?((!%_IsSmi(%IS_VAR(g))&&!(g==g))?g:(Modulo(g,1000))):ToNumber(d);
	var i=MakeTime((Modulo($floor(g/3600000),24)),a,b,d);
	return %_SetValueOf(this,TimeClip(UTC(MakeDate(($floor(g/86400000)),i))));
}



function DateSetUTCMinutes(a,b,d){
	var g=(%_ClassOf(this)==='Date'?%_ValueOf(this):ThrowDateTypeError());
	a=ToNumber(a);
	var h=%_ArgumentsLength();
	b=h<2?((!%_IsSmi(%IS_VAR(g))&&!(g==g))?g:(Modulo($floor(g/1000),60))):ToNumber(b);
	d=h<3?((!%_IsSmi(%IS_VAR(g))&&!(g==g))?g:(Modulo(g,1000))):ToNumber(d);
	var i=MakeTime((Modulo($floor(g/3600000),24)),a,b,d);
	return %_SetValueOf(this,TimeClip(MakeDate(($floor(g/86400000)),i)));
}



function DateSetHours(a,b,d,g){
	var h=LocalTime((%_ClassOf(this)==='Date'?%_ValueOf(this):ThrowDateTypeError()));
	a=ToNumber(a);
	var i=%_ArgumentsLength();
	b=i<2?((!%_IsSmi(%IS_VAR(h))&&!(h==h))?h:(Modulo($floor(h/60000),60))):ToNumber(b);
	d=i<3?((!%_IsSmi(%IS_VAR(h))&&!(h==h))?h:(Modulo($floor(h/1000),60))):ToNumber(d);
	g=i<4?((!%_IsSmi(%IS_VAR(h))&&!(h==h))?h:(Modulo(h,1000))):ToNumber(g);
	var j=MakeTime(a,b,d,g);
	return %_SetValueOf(this,TimeClip(UTC(MakeDate(($floor(h/86400000)),j))));
}



function DateSetUTCHours(a,b,d,g){
	var h=(%_ClassOf(this)==='Date'?%_ValueOf(this):ThrowDateTypeError());
	a=ToNumber(a);
	var i=%_ArgumentsLength();
	b=i<2?((!%_IsSmi(%IS_VAR(h))&&!(h==h))?h:(Modulo($floor(h/60000),60))):ToNumber(b);
	d=i<3?((!%_IsSmi(%IS_VAR(h))&&!(h==h))?h:(Modulo($floor(h/1000),60))):ToNumber(d);
	g=i<4?((!%_IsSmi(%IS_VAR(h))&&!(h==h))?h:(Modulo(h,1000))):ToNumber(g);
	var j=MakeTime(a,b,d,g);
	return %_SetValueOf(this,TimeClip(MakeDate(($floor(h/86400000)),j)));
}



function DateSetDate(a){
	var b=LocalTime((%_ClassOf(this)==='Date'?%_ValueOf(this):ThrowDateTypeError()));
	a=ToNumber(a);
	var d=MakeDay((YearFromTime(b)),(MonthFromTime(b)),a);
	return %_SetValueOf(this,TimeClip(UTC(MakeDate(d,TimeWithinDay(b)))));
}



function DateSetUTCDate(a){
	var b=(%_ClassOf(this)==='Date'?%_ValueOf(this):ThrowDateTypeError());
	a=ToNumber(a);
	var d=MakeDay((YearFromTime(b)),(MonthFromTime(b)),a);
	return %_SetValueOf(this,TimeClip(MakeDate(d,TimeWithinDay(b))));
}



function DateSetMonth(a,b){
	var d=LocalTime((%_ClassOf(this)==='Date'?%_ValueOf(this):ThrowDateTypeError()));
	a=ToNumber(a);
	b=%_ArgumentsLength()<2?((!%_IsSmi(%IS_VAR(d))&&!(d==d))?d:(DateFromTime(d))):ToNumber(b);
	var g=MakeDay((YearFromTime(d)),a,b);
	return %_SetValueOf(this,TimeClip(UTC(MakeDate(g,TimeWithinDay(d)))));
}



function DateSetUTCMonth(a,b){
	var d=(%_ClassOf(this)==='Date'?%_ValueOf(this):ThrowDateTypeError());
	a=ToNumber(a);
	b=%_ArgumentsLength()<2?((!%_IsSmi(%IS_VAR(d))&&!(d==d))?d:(DateFromTime(d))):ToNumber(b);
	var g=MakeDay((YearFromTime(d)),a,b);
	return %_SetValueOf(this,TimeClip(MakeDate(g,TimeWithinDay(d))));
}



function DateSetFullYear(a,b,d){
	var g=(%_ClassOf(this)==='Date'?%_ValueOf(this):ThrowDateTypeError());
	g=(!%_IsSmi(%IS_VAR(g))&&!(g==g))?0:LocalTimeNoCheck(g);
	a=ToNumber(a);
	var h=%_ArgumentsLength();
	b=h<2?(MonthFromTime(g)):ToNumber(b);
	d=h<3?(DateFromTime(g)):ToNumber(d);
	var i=MakeDay(a,b,d);
	return %_SetValueOf(this,TimeClip(UTC(MakeDate(i,TimeWithinDay(g)))));
}



function DateSetUTCFullYear(a,b,d){
	var g=(%_ClassOf(this)==='Date'?%_ValueOf(this):ThrowDateTypeError());
	if((!%_IsSmi(%IS_VAR(g))&&!(g==g)))g=0;
	var h=%_ArgumentsLength();
	a=ToNumber(a);
	b=h<2?(MonthFromTime(g)):ToNumber(b);
	d=h<3?(DateFromTime(g)):ToNumber(d);
	var i=MakeDay(a,b,d);
	return %_SetValueOf(this,TimeClip(MakeDate(i,TimeWithinDay(g))));
}



function DateToUTCString(){
	var a=(%_ClassOf(this)==='Date'?%_ValueOf(this):ThrowDateTypeError());
	if((!%_IsSmi(%IS_VAR(a))&&!(a==a)))return'Invalid Date';

	return WeekDays[WeekDay(a)]+', '
		+TwoDigitString((DateFromTime(a)))+' '
		+Months[(MonthFromTime(a))]+' '
		+(YearFromTime(a))+' '
		+TimeString(a)+' GMT';
}



function DateGetYear(){
	var a=(%_ClassOf(this)==='Date'?%_ValueOf(this):ThrowDateTypeError());
	if((!%_IsSmi(%IS_VAR(a))&&!(a==a)))return $NaN;
	return(YearFromTime(LocalTimeNoCheck(a)))-1900;
}



function DateSetYear(a){
	var b=LocalTime((%_ClassOf(this)==='Date'?%_ValueOf(this):ThrowDateTypeError()));
	if((!%_IsSmi(%IS_VAR(b))&&!(b==b)))b=0;
	a=ToNumber(a);
	if((!%_IsSmi(%IS_VAR(a))&&!(a==a)))return %_SetValueOf(this,$NaN);
	a=(0<=(%_IsSmi(%IS_VAR(a))?a:%NumberToInteger(ToNumber(a)))&&(%_IsSmi(%IS_VAR(a))?a:%NumberToInteger(ToNumber(a)))<=99)
		?1900+(%_IsSmi(%IS_VAR(a))?a:%NumberToInteger(ToNumber(a))):a;
	var d=MakeDay(a,(MonthFromTime(b)),(DateFromTime(b)));
	return %_SetValueOf(this,TimeClip(UTC(MakeDate(d,TimeWithinDay(b)))));
}









function DateToGMTString(){
	return DateToUTCString.call(this);
}


function PadInt(a,b){
	if(b==1)return a;
	return a<MathPow(10,b-1)?'0'+PadInt(a,b-1):a;
}


function DateToISOString(){
	var a=(%_ClassOf(this)==='Date'?%_ValueOf(this):ThrowDateTypeError());
	if((!%_IsSmi(%IS_VAR(a))&&!(a==a)))return'Invalid Date';
	return this.getUTCFullYear()+'-'+PadInt(this.getUTCMonth()+1,2)+
		'-'+PadInt(this.getUTCDate(),2)+'T'+PadInt(this.getUTCHours(),2)+
		':'+PadInt(this.getUTCMinutes(),2)+':'+PadInt(this.getUTCSeconds(),2)+
		'.'+PadInt(this.getUTCMilliseconds(),3)+
		'Z';
}


function DateToJSON(a){
	return CheckJSONPrimitive(this.toISOString());
}




function SetupDate(){

	InstallFunctions($Date,2,$Array(
				"UTC",DateUTC,
				"parse",DateParse,
				"now",DateNow
				));


	%SetProperty($Date.prototype,"constructor",$Date,2);



	InstallFunctionsOnHiddenPrototype($Date.prototype,2,$Array(
				"toString",DateToString,
				"toDateString",DateToDateString,
				"toTimeString",DateToTimeString,
				"toLocaleString",DateToLocaleString,
				"toLocaleDateString",DateToLocaleDateString,
				"toLocaleTimeString",DateToLocaleTimeString,
				"valueOf",DateValueOf,
				"getTime",DateGetTime,
				"getFullYear",DateGetFullYear,
				"getUTCFullYear",DateGetUTCFullYear,
				"getMonth",DateGetMonth,
				"getUTCMonth",DateGetUTCMonth,
				"getDate",DateGetDate,
				"getUTCDate",DateGetUTCDate,
				"getDay",DateGetDay,
				"getUTCDay",DateGetUTCDay,
				"getHours",DateGetHours,
				"getUTCHours",DateGetUTCHours,
				"getMinutes",DateGetMinutes,
				"getUTCMinutes",DateGetUTCMinutes,
				"getSeconds",DateGetSeconds,
				"getUTCSeconds",DateGetUTCSeconds,
				"getMilliseconds",DateGetMilliseconds,
				"getUTCMilliseconds",DateGetUTCMilliseconds,
				"getTimezoneOffset",DateGetTimezoneOffset,
				"setTime",DateSetTime,
				"setMilliseconds",DateSetMilliseconds,
				"setUTCMilliseconds",DateSetUTCMilliseconds,
				"setSeconds",DateSetSeconds,
				"setUTCSeconds",DateSetUTCSeconds,
				"setMinutes",DateSetMinutes,
				"setUTCMinutes",DateSetUTCMinutes,
				"setHours",DateSetHours,
				"setUTCHours",DateSetUTCHours,
				"setDate",DateSetDate,
				"setUTCDate",DateSetUTCDate,
				"setMonth",DateSetMonth,
				"setUTCMonth",DateSetUTCMonth,
				"setFullYear",DateSetFullYear,
				"setUTCFullYear",DateSetUTCFullYear,
				"toGMTString",DateToGMTString,
				"toUTCString",DateToUTCString,
				"getYear",DateGetYear,
				"setYear",DateSetYear,
				"toISOString",DateToISOString,
				"toJSON",DateToJSON
					));
}

SetupDate();































const $RegExp=global.RegExp;



function DoConstructRegExp(a,b,d,g){

	if((%_IsRegExp(b))){
		if(!(typeof(d)==='undefined')){
			throw MakeTypeError('regexp_flags',[]);
		}
		d=(b.global?'g':'')
			+(b.ignoreCase?'i':'')
			+(b.multiline?'m':'');
		b=b.source;
	}

	b=(typeof(b)==='undefined')?'':ToString(b);
	d=(typeof(d)==='undefined')?'':ToString(d);

	var h=false;
	var i=false;
	var j=false;

	for(var k=0;k<d.length;k++){
		var l=StringCharAt.call(d,k);
		switch(l){
			case'g':

				h=true;
				break;
			case'i':
				i=true;
				break;
			case'm':
				j=true;
				break;
			default:


				break;
		}
	}

	if(!g){
		regExpCache.type='none';
	}
	%RegExpInitializeObject(a,b,h,i,j);


	%RegExpCompile(a,b,d);
}


function RegExpConstructor(a,b){
	if(%_IsConstructCall()){
		DoConstructRegExp(this,a,b,true);
	}else{

		if((%_IsRegExp(a))&&(typeof(b)==='undefined')){
			return a;
		}
		return new $RegExp(a,b);
	}
}






function CompileRegExp(a,b){







	if((typeof(a)==='undefined')&&%_ArgumentsLength()!=0){
		DoConstructRegExp(this,'undefined',b,false);
	}else{
		DoConstructRegExp(this,a,b,false);
	}
}


function DoRegExpExec(a,b,d){
	var g=%_RegExpExec(a,b,d,lastMatchInfo);
	if(g!==null)lastMatchInfoOverride=null;
	return g;
}


function RegExpCache(){
	this.type='none';
	this.regExp=0;
	this.subject=0;
	this.replaceString=0;
	this.lastIndex=0;
	this.answer=0;


	this.answerSaved=false;
}


var regExpCache=new RegExpCache();


function BuildResultFromMatchInfo(a,b){
	var d=((a)[0])>>1;
	var g=%_RegExpConstructResult(d,a[3],b);
	if(d===1){
		var h=a[(3+(0))];
		var i=a[(3+(1))];
		g[0]=SubString(b,h,i);
	}else{
		for(var j=0;j<d;j++){
			var h=a[(3+(j<<1))];
			var i=a[(3+((j<<1)+1))];
			if(h!=-1&&i!=-1){
				g[j]=SubString(b,h,i);
			}else{


				g[j]=void 0;
			}
		}
	}
	return g;
}


function RegExpExecNoTests(a,b,d){

	var g=DoRegExpExec(a,b,d);
	var h=null;
	if(g!==null){
		h=BuildResultFromMatchInfo(g,b);
	}
	return h;
}


function RegExpExec(a){
	if(!(%_IsRegExp(this))){
		throw MakeTypeError('incompatible_method_receiver',
				['RegExp.prototype.exec',this]);
	}

	var b=regExpCache;
	var d=false;

	if(%_ObjectEquals(b.type,'exec')&&
			%_ObjectEquals(b.lastIndex,this.lastIndex)&&
			%_IsRegExpEquivalent(b.regExp,this)&&
			%_ObjectEquals(b.subject,a)){
				if(b.answerSaved){



					if(!this.global)this.lastIndex=lastMatchInfo[4];
					return %_RegExpCloneResult(b.answer);
				}else{
					d=true;
				}
			}

	if(%_ArgumentsLength()==0){
		var g=((lastMatchInfo)[2]);
		if((typeof(g)==='undefined')){
			throw MakeError('no_input_to_regexp',[this]);
		}
		a=g;
	}
	var h;
	if((typeof(a)==='string')){
		h=a;
	}else{
		h=ToString(a);
	}
	var i=this.lastIndex;

	var j=this.global?(%_IsSmi(%IS_VAR(i))?i:%NumberToInteger(ToNumber(i))):0;

	if(j<0||j>h.length){
		this.lastIndex=0;
		return null;
	}

	%_Log('regexp','regexp-exec,%0r,%1S,%2i',[this,h,i]);

	var k=%_RegExpExec(this,h,j,lastMatchInfo);

	if(k==null){
		if(this.global){
			this.lastIndex=0;
			if(i!=0)return k;
		}
		b.lastIndex=i;
		b.regExp=this;
		b.subject=h;
		b.answer=k;
		b.answerSaved=true;
		b.type='exec';
		return k;
	}
	lastMatchInfoOverride=null;
	var l=BuildResultFromMatchInfo(k,h);

	if(this.global){
		this.lastIndex=lastMatchInfo[4];
	}else{
		b.regExp=this;
		b.subject=h;
		b.lastIndex=i;
		if(d)b.answer=%_RegExpCloneResult(l);
		b.answerSaved=d;
		b.type='exec';
	}
	return l;

}



var regexp_key;
var regexp_val;





function RegExpTest(a){
	if(!(%_IsRegExp(this))){
		throw MakeTypeError('incompatible_method_receiver',
				['RegExp.prototype.test',this]);
	}
	if(%_ArgumentsLength()==0){
		var b=((lastMatchInfo)[2]);
		if((typeof(b)==='undefined')){
			throw MakeError('no_input_to_regexp',[this]);
		}
		a=b;
	}
	var d;
	if((typeof(a)==='string')){
		d=a;
	}else{
		d=ToString(a);
	}

	var g=this.lastIndex;
	var h=regExpCache;
	if(%_ObjectEquals(h.type,'test')&&
			%_IsRegExpEquivalent(h.regExp,this)&&
			%_ObjectEquals(h.subject,a)&&
			%_ObjectEquals(h.lastIndex,g)){



				if(this.global)this.lastIndex=lastMatchInfo[4];
				return h.answer;
			}




	if(%_StringCharCodeAt(this.source,0)==46&&
			%_StringCharCodeAt(this.source,1)==42&&
			%_StringCharCodeAt(this.source,2)!=63){
				if(!%_ObjectEquals(regexp_key,this)){
					regexp_key=this;
					regexp_val=new $RegExp(this.source.substring(2,this.source.length),
							(this.global?'g':'')
							+(this.ignoreCase?'i':'')
							+(this.multiline?'m':''));
				}
				if(!regexp_val.test(d))return false;
			}

	var i=d.length;
	var j=this.global?(%_IsSmi(%IS_VAR(g))?g:%NumberToInteger(ToNumber(g))):0;

	h.type='test';
	h.regExp=this;
	h.subject=d;
	h.lastIndex=j;

	if(j<0||j>i){
		this.lastIndex=0;
		h.answer=false;
		return false;
	}

	%_Log('regexp','regexp-exec,%0r,%1S,%2i',[this,d,g]);

	var k=%_RegExpExec(this,d,j,lastMatchInfo);

	if(k==null){
		if(this.global)this.lastIndex=0;
		h.answer=false;
		return false;
	}
	lastMatchInfoOverride=null;
	if(this.global)this.lastIndex=lastMatchInfo[4];
	h.answer=true;
	return true;
}


function RegExpToString(){



	var a=this.source?this.source:'(?:)';
	var b='/'+a+'/';
	if(this.global)
		b+='g';
	if(this.ignoreCase)
		b+='i';
	if(this.multiline)
		b+='m';
	return b;
}






function RegExpGetLastMatch(){
	if(lastMatchInfoOverride!==null){
		return lastMatchInfoOverride[0];
	}
	var a=((lastMatchInfo)[1]);
	return SubString(a,
			lastMatchInfo[3],
			lastMatchInfo[4]);
}


function RegExpGetLastParen(){
	if(lastMatchInfoOverride){
		var a=lastMatchInfoOverride;
		if(a.length<=3)return'';
		return a[a.length-3];
	}
	var b=((lastMatchInfo)[0]);
	if(b<=2)return'';



	var d=((lastMatchInfo)[1]);
	var g=lastMatchInfo[(3+(b-2))];
	var h=lastMatchInfo[(3+(b-1))];
	if(g!=-1&&h!=-1){
		return SubString(d,g,h);
	}
	return"";
}


function RegExpGetLeftContext(){
	var a;
	var b;
	if(!lastMatchInfoOverride){
		a=lastMatchInfo[3];
		b=((lastMatchInfo)[1]);
	}else{
		var d=lastMatchInfoOverride;
		a=d[d.length-2];
		b=d[d.length-1];
	}
	return SubString(b,0,a);
}


function RegExpGetRightContext(){
	var a;
	var b;
	if(!lastMatchInfoOverride){
		a=lastMatchInfo[4];
		b=((lastMatchInfo)[1]);
	}else{
		var d=lastMatchInfoOverride;
		b=d[d.length-1];
		a=d[d.length-2]+b.length;
	}
	return SubString(b,a,b.length);
}





function RegExpMakeCaptureGetter(a){
	return function(){
		if(lastMatchInfoOverride){
			if(a<lastMatchInfoOverride.length-2)return lastMatchInfoOverride[a];
			return'';
		}
		var b=a*2;
		if(b>=((lastMatchInfo)[0]))return'';
		var d=lastMatchInfo[(3+(b))];
		var g=lastMatchInfo[(3+(b+1))];
		if(d==-1||g==-1)return'';
		return SubString(((lastMatchInfo)[1]),d,g);
	};
}








var lastMatchInfo=[
2,
	"",
	void 0,
	0,
	0,
	];





var lastMatchInfoOverride=null;



function SetupRegExp(){
	%FunctionSetInstanceClassName($RegExp,'RegExp');
	%FunctionSetPrototype($RegExp,new $Object());
	%SetProperty($RegExp.prototype,'constructor',$RegExp,2);
	%SetCode($RegExp,RegExpConstructor);

	InstallFunctions($RegExp.prototype,2,$Array(
				"exec",RegExpExec,
				"test",RegExpTest,
				"toString",RegExpToString,
				"compile",CompileRegExp
				));


	%FunctionSetLength($RegExp.prototype.compile,1);




	function RegExpGetInput(){
		var a=((lastMatchInfo)[2]);
		return(typeof(a)==='undefined')?"":a;
	}
	function RegExpSetInput(b){
		regExpCache.type='none';
		((lastMatchInfo)[2])=ToString(b);
	};

	%DefineAccessor($RegExp,'input',0,RegExpGetInput,4);
	%DefineAccessor($RegExp,'input',1,RegExpSetInput,4);
	%DefineAccessor($RegExp,'$_',0,RegExpGetInput,2|4);
	%DefineAccessor($RegExp,'$_',1,RegExpSetInput,2|4);
	%DefineAccessor($RegExp,'$input',0,RegExpGetInput,2|4);
	%DefineAccessor($RegExp,'$input',1,RegExpSetInput,2|4);









	var d=false;
	function RegExpGetMultiline(){return d;};
	function RegExpSetMultiline(g){d=g?true:false;};

	%DefineAccessor($RegExp,'multiline',0,RegExpGetMultiline,4);
	%DefineAccessor($RegExp,'multiline',1,RegExpSetMultiline,4);
	%DefineAccessor($RegExp,'$*',0,RegExpGetMultiline,2|4);
	%DefineAccessor($RegExp,'$*',1,RegExpSetMultiline,2|4);


	function NoOpSetter(h){}



	%DefineAccessor($RegExp,'lastMatch',0,RegExpGetLastMatch,4);
	%DefineAccessor($RegExp,'lastMatch',1,NoOpSetter,4);
	%DefineAccessor($RegExp,'$&',0,RegExpGetLastMatch,2|4);
	%DefineAccessor($RegExp,'$&',1,NoOpSetter,2|4);
	%DefineAccessor($RegExp,'lastParen',0,RegExpGetLastParen,4);
	%DefineAccessor($RegExp,'lastParen',1,NoOpSetter,4);
	%DefineAccessor($RegExp,'$+',0,RegExpGetLastParen,2|4);
	%DefineAccessor($RegExp,'$+',1,NoOpSetter,2|4);
	%DefineAccessor($RegExp,'leftContext',0,RegExpGetLeftContext,4);
	%DefineAccessor($RegExp,'leftContext',1,NoOpSetter,4);
	%DefineAccessor($RegExp,'$`',0,RegExpGetLeftContext,2|4);
	%DefineAccessor($RegExp,'$`',1,NoOpSetter,2|4);
	%DefineAccessor($RegExp,'rightContext',0,RegExpGetRightContext,4);
	%DefineAccessor($RegExp,'rightContext',1,NoOpSetter,4);
	%DefineAccessor($RegExp,"$'",0,RegExpGetRightContext,2|4);
	%DefineAccessor($RegExp,"$'",1,NoOpSetter,2|4);

	for(var i=1;i<10;++i){
		%DefineAccessor($RegExp,'$'+i,0,RegExpMakeCaptureGetter(i),4);
		%DefineAccessor($RegExp,'$'+i,1,NoOpSetter,4);
	}
}


SetupRegExp();




























var $JSON=global.JSON;

function ParseJSONUnfiltered(a){
	var b=$String(a);
	var d=%CompileString(b,true);
	return d();
}

function Revive(a,b,d){
	var g=a[b];
	if((%_IsObject(g))){
		if((%_IsArray(g))){
			var h=g.length;
			for(var i=0;i<h;i++){
				var j=Revive(g,$String(i),d);
				g[i]=j;
			}
		}else{
			for(var k in g){
				if(ObjectHasOwnProperty.call(g,k)){
					var j=Revive(g,k,d);
					if((typeof(j)==='undefined')){
						delete g[k];
					}else{
						g[k]=j;
					}
				}
			}
		}
	}
	return d.call(a,b,g);
}

function JSONParse(a,b){
	var d=ParseJSONUnfiltered(a);
	if((%_IsFunction(b))){
		return Revive({'':d},'',b);
	}else{
		return d;
	}
}

var characterQuoteCache={
	'\b':'\\b',
	'\t':'\\t',
	'\n':'\\n',
	'\f':'\\f',
	'\r':'\\r',
	'\"':'\\"',
	'\\':'\\\\'
};

function QuoteSingleJSONCharacter(a){
	if(a in characterQuoteCache){
		return characterQuoteCache[a];
	}
	var b=a.charCodeAt(0);
	var d;
	if(b<16)d='\\u000';
	else if(b<256)d='\\u00';
	else if(b<4096)d='\\u0';
	else d='\\u';
	d+=b.toString(16);
	characterQuoteCache[a]=d;
	return d;
}

function QuoteJSONString(a){
	var b=/[\\\"\x00-\x1f]/g;
	return'"'+a.replace(b,QuoteSingleJSONCharacter)+'"';
}

function StackContains(a,b){
	var d=a.length;
	for(var g=0;g<d;g++){
		if(a[g]===b){
			return true;
		}
	}
	return false;
}

function SerializeArray(a,b,d,g,h){
	if(StackContains(d,a)){
		throw MakeTypeError('circular_structure',[]);
	}
	d.push(a);
	var i=g;
	g+=h;
	var j=[];
	var k=a.length;
	for(var l=0;l<k;l++){
		var m=JSONSerialize($String(l),a,b,d,
				g,h);
		if((typeof(m)==='undefined')){
			m="null";
		}
		j.push(m);
	}
	var o;
	if(h==""){
		o="["+j.join(",")+"]";
	}else if(j.length>0){
		var q=",\n"+g;
		o="[\n"+g+j.join(q)+"\n"+
			i+"]";
	}else{
		o="[]";
	}
	d.pop();
	return o;
}

function SerializeObject(a,b,d,g,h){
	if(StackContains(d,a)){
		throw MakeTypeError('circular_structure',[]);
	}
	d.push(a);
	var i=g;
	g+=h;
	var j=[];
	if((%_IsArray(b))){
		var k=b.length;
		for(var l=0;l<k;l++){
			if(ObjectHasOwnProperty.call(b,l)){
				var m=b[l];
				var o=JSONSerialize(m,a,b,d,g,h);
				if(!(typeof(o)==='undefined')){
					var q=QuoteJSONString(m)+":";
					if(h!="")q+=" ";
					q+=o;
					j.push(q);
				}
			}
		}
	}else{
		for(var m in a){
			if(ObjectHasOwnProperty.call(a,m)){
				var o=JSONSerialize(m,a,b,d,g,h);
				if(!(typeof(o)==='undefined')){
					var q=QuoteJSONString(m)+":";
					if(h!="")q+=" ";
					q+=o;
					j.push(q);
				}
			}
		}
	}
	var r;
	if(h==""){
		r="{"+j.join(",")+"}";
	}else if(j.length>0){
		var u=",\n"+g;
		r="{\n"+g+j.join(u)+"\n"+
			i+"}";
	}else{
		r="{}";
	}
	d.pop();
	return r;
}

function JSONSerialize(a,b,d,g,h,i){
	var j=b[a];
	if((%_IsObject(j))&&j){
		var k=j.toJSON;
		if((%_IsFunction(k))){
			j=k.call(j,a);
		}
	}
	if((%_IsFunction(d))){
		j=d.call(b,a,j);
	}

	if((%_IsObject(j))){
		if((%_ClassOf(j)==='Number')){
			j=$Number(j);
		}else if((%_ClassOf(j)==='String')){
			j=$String(j);
		}else if((%_ClassOf(j)==='Boolean')){
			j=%_ValueOf(j);
		}
	}
	switch(typeof j){
		case"string":
			return QuoteJSONString(j);
		case"object":
			if(!j){
				return"null";
			}else if((%_IsArray(j))){
				return SerializeArray(j,d,g,h,i);
			}else{
				return SerializeObject(j,d,g,h,i);
			}
		case"number":
			return $isFinite(j)?$String(j):"null";
		case"boolean":
			return j?"true":"false";
	}
}

function JSONStringify(a,b,d){
	var g=[];
	var h="";
	if((%_IsObject(d))){

		if((%_ClassOf(d)==='Number')){
			d=$Number(d);
		}else if((%_ClassOf(d)==='String')){
			d=$String(d);
		}
	}
	var i;
	if((typeof(d)==='number')){
		d=$Math.min(ToInteger(d),10);
		i="";
		for(var j=0;j<d;j++){
			i+=" ";
		}
	}else if((typeof(d)==='string')){
		if(d.length>10){
			i=d.substring(0,10);
		}else{
			i=d;
		}
	}else{
		i="";
	}
	return JSONSerialize('',{'':a},b,g,h,i);
}

function SetupJSON(){
	InstallFunctions($JSON,2,$Array(
				"parse",JSONParse,
				"stringify",JSONStringify
				));
}

SetupJSON();
