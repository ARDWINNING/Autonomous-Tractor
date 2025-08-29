from setuptools import find_packages, setup

package_name = 'Field_Pathfinding_Srv'

setup(
    name=package_name,
    version='0.0.0',
    packages=find_packages(exclude=['test']),
    data_files=[
        ('share/ament_index/resource_index/packages',
            ['resource/' + package_name]),
        ('share/' + package_name, ['package.xml']),
    ],
    install_requires=['setuptools'],
    zip_safe=True,
    maintainer='Alexander Winning',
    maintainer_email='ardwinning@gmail.com',
    description='Pathfinding for fields. Inputs: FieldID, Turning Radius, Implement Width. Outputs X Coordinate List, Y Coordinate List.'
    license='Apache-2.0',
    tests_require=['pytest'],
    entry_points={
        'console_scripts': [
            'service = Field_Pathfinding_Srv.service_member_function:main',    
        ],
    },
)
